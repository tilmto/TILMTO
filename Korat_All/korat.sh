#!/bin/bash
#The following command are provided by Korat official website
#More information: http://korat.sourceforge.net/manual.html 

echo "Command line: java -cp korat.jar korat.Korat";
java -cp korat.jar korat.Korat --help  #the bash shows expected help information

echo -e "\n\nCommand line: java -cp korat.jar korat.Korat --visualize --class korat.examples.binarytree.BinaryTree --args 3,3,3";
java -cp korat.jar korat.Korat --visualize --class korat.examples.binarytree.BinaryTree --args 3,3,3  # cannot find the class 
