#!/bin/bash
# Script for installing the nodes


function installNode {

if test -e ./nodeprog; then
	echo "nodeprog exists"
	version=\"$1\"
	output=$(./nodeprog -version 2>&1)
	echo "Version:"$version
	echo "output:"$output
	if [[ $output =~ .*$version ]]; then
		echo 'Node already exists with correct version'
		return
		#sudo nohup bash run.sh >/dev/null 2>&1 &	
	fi
fi
wget -N --cache=off toki.dlinkddns.com/master/nodeprog
chmod +x ./nodeprog
sudo yum -y install bind-utils #todo
sudo yum -y install ping
#sudo nohup bash run.sh >/dev/null 2>&1 &
}

function runNode
{
	while true; do
		sudo ./nodeprog 33555
		exitCode=$?
		if [ $exitCode -eq 1 ]; then
			echo "Normal termination"
			break;
		elif [ $exitCode -eq 13 ]; then
			echo "Doing a reinstall"
			installNode
		else
			echo "Error termination:"$exitCode
			echo "Restarting"
		fi
	done
	echo "run script finished"
}



installNode $1
runNode

echo "install script finished"
