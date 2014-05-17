#!/bin/bash
# Script for installing the nodes

function installNodeProg
{
	sudo yum -y install bind-utils #todo
	sudo yum -y install iperf
	sudo yum -y install ping
	echo "Installing from"$1
	wget -N --cache=off $1/nodeprog
	chmod +x ./nodeprog
	#sudo nohup bash run.sh >/dev/null 2>&1 &
}

function prepareNodeProg
{
	if test -e ./nodeprog; then
		echo "nodeprog exists"
		version=$1
		output=$(./nodeprog -v 2>&1)
		arr=($output)
		output2=${arr[1]}
		echo "Version:"$version
		echo "output:"$output2
		if (( $output2 == $version )); then
			echo 'Node already exists with correct version'
			return
			#sudo nohup bash run.sh >/dev/null 2>&1 &
		fi
		echo 'Updating node'
	fi
	installNodeProg $2
}

function runNodeProg
{
	while true; do
		if [ -z "$1" ]; then
			./nodeprog -p 33555
		else
			./nodeprog -p 33555 -r $1
		fi
		exitCode=$?
		if [ $exitCode -eq 1 ]; then
			echo "Normal termination"
			break;
		elif [ $exitCode -eq 2 ]; then
			echo "Doing a reinstall"
			installNodeProg
		elif [ $exitCode -eq 3 ]; then
			echo "Doing a restart:"$exitCode
		else
			echo "Error termination";
			break;
		fi
	done
	echo "run script finished"
}

if [ "$(pidof nodeprog)" ]; then
  echo "Process already running";
  exit;
else
  echo "Process not running";
fi

prepareNodeProg $1 $2

runNodeProg $3

echo "install script finished"
