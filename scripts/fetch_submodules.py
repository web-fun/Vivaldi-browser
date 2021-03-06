from __future__ import print_function

import os
import sys
import subprocess

import tools.gittools.gittools as Git
from git_urls import BASE_URL, CHROMIUM_URL

SRC = os.path.dirname(os.path.dirname(__file__))

BLACKLISTED_MODULES = []
BLACKLISTED_OS = ["android_sdk_sources"]
BLACKLISTED_OS_CONDITION = ["checkout_"+x for x in BLACKLISTED_OS]
BASE_URL= "ssh://git.viv.int/srv/git/vivaldi"
CHROMIUM_URL = BASE_URL + "/chromium/src.git"

def __GetModuleInfoFromDeps(deps_info, selected_os=None, git=None):
  submodules = {}
  for mod, ref in deps_info["deps"].items():
    #print mod, ref
    if mod.startswith("src/"):
      mod = mod[4:] # remove "src/"
    mod.replace("\\", "/")
    if ref == None:
      continue

    if isinstance(ref, dict):
      if "condition" in ref:
        condition = ref["condition"]
        if any([cond in condition for cond in BLACKLISTED_OS_CONDITION]):
          continue
        if selected_os not in condition:
          continue
      if ref.get("dep_type",None) == "cipd":
        submodules.setdefault("__cipd__", {})[mod] = ref.get("packages",[])
        continue
      if "url" not in ref:
        continue
      ref = ref["url"]
    elif selected_os:
       continue # only check out the selected OS when it is selected

    url, commit = ref.split("@")[:2]
    if commit != None and mod not in BLACKLISTED_MODULES:
      submodules[mod] = {
            "url":git.ResolveURL(url, True) if git else url,
            "commit":commit
          }

  return submodules

def GetSubmodules(checkout_platform, checkout_filter=None):
  chromium_git = Git.Git(source_dir=os.path.join(SRC, "chromium"),
                         url = CHROMIUM_URL,
                         base_url = BASE_URL)

  deps_info = chromium_git.GetGitDepInfo()
  submodules = __GetModuleInfoFromDeps(deps_info, checkout_platform, chromium_git)
  def _cipd(mod_packages):
    #for path, packages in mod_packages.iteritems():
    #  print path,":", packages
    ensure_file = ""
    ensure_file += "$ParanoidMode CheckPresence\n"
    for path, packages in mod_packages.items():
      ensure_file += "@Subdir %s\n" % path
      for package in packages:
        ensure_file += "%s %s\n" % (package["package"], package.get("version", ""))
    #print "==========="
    #print ensure_file
    #print "==========="
    cmd = [
          os.path.join(SRC, "chromium", "third_party", "depot_tools", "cipd"),
          "ensure",
          "-log-level", "info",
          "-root",  os.path.join(SRC, "chromium"),
          "-cache-dir", os.path.join(SRC, "chromium", ".cipd"),
          "-ensure-file=-"
          ]
    print(os.getcwd(),":", " ".join(cmd))
    command = subprocess.Popen(cmd, stdin=subprocess.PIPE)
    command.communicate(ensure_file)
    if command.returncode != 0:
      raise Exception("Command failed with exit code %d"% command.returncode)

  def _checkout_modules(_submodules):
    for path, config in _submodules.items():
      if path == "__cipd__":
        continue;
      module_git = Git.Git(inherit=chromium_git, url=config["url"],
                          source_dir = os.path.join(SRC, "chromium", path))
      module_git.FetchSource(update_submodules=False)
      module_git.Checkout(revision=config["commit"], only_commit=True,
                          update_submodules=False)

    if "__cipd__" in _submodules:
      _cipd(_submodules["__cipd__"])

  _checkout_modules(checkout_filter(submodules) if checkout_filter else submodules)

  main_submodules = dict(submodules)
  submodules = {}
  for path in deps_info["recursedeps"]:
    deps_file = "DEPS"
    if isinstance(path, (list, tuple)):
      path, deps_file = path[:2]
    if path.startswith("src/"):
      path = path[4:] # remove "src/"
    print(path)
    if path not in main_submodules:
      continue

    submodule_git = Git.Git(inherit=chromium_git,
                         source_dir=os.path.join(SRC, "chromium", path),
                         url = main_submodules[path]["url"],
                         base_url = BASE_URL)

    sub_deps_info = submodule_git.GetGitDepInfo(filename=deps_file)
    use_relative_paths = sub_deps_info.get("use_relative_paths", False)
    additional_modules = __GetModuleInfoFromDeps(sub_deps_info, checkout_platform, submodule_git)
    for sub_path, config in additional_modules.items():
      submodules[sub_path if use_relative_paths else os.path.join(path, sub_path)] = config

  _checkout_modules(checkout_filter(submodules) if checkout_filter else submodules)

  return 0

if __name__ == '__main__':
  sys.exit(main())