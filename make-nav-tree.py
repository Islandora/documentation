#!/usr/local/bin/python3

import yaml
from treelib import Node, Tree

def expand_yaml_to_tree(data, tree, parent = None):
  label = list(data.keys())[0]
  if parent == None:
    tree.create_node(label, label)
  else:
    tree.create_node(label, label, parent = parent)
  if type(data[label]) == list:
    for branch in data[label]:
      tree = expand_yaml_to_tree(branch, tree, label)
  return tree

with open('mkdocs.yml','r') as mkdocs:
  full_file_contents = yaml.safe_load(mkdocs)
  forest = []
  for top_level_item in full_file_contents['nav']:
    tree = Tree()
    tree = expand_yaml_to_tree(top_level_item, tree)
    forest.append(tree)

for tree in forest:
  tree.show()
