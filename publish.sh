#Check input
if [ -z "$*" ]; then
  echo "\033[31mPlease input version!\033[0m"
  exit
fi

#Syntax of version
ver=$(expr $* : '^\([0-9]*[0-9][.][0-9]*[0-9][.][0-9]*[0-9]\)')
if [ "$ver" != "$*" ]; then
  echo "\033[31mInvalid version!\033[0m"
  exit
fi

#Check podspec
filename=$(find . -depth 1 -name *.podspec)
filecontent=$(cat $filename)
location=$(echo "$filecontent" |grep $ver)
location=$(expr "$location" : ".*[\",\']\(.*\)[\",\']")
if [ "$location" != "$ver" ]; then
  echo "\033[31mCan not find $ver in podspec!\033[0m"
  exit
fi

#Check commit
echo "\nTry to git fetch && rebase..."
git fetch
git rebase
gitstatus=$(git status)
clean=$(echo $gitstatus |grep "nothing to commit")

if [ -z "$clean" ]; then
  echo "\033[31mMaybe you should commit all changes first.\033[0m"
fi

#User confirm
echo "\nVersion(\033[31m$ver\033[0m) will be added. Sure? [Y/n]"
read user_ensure

if [ "$user_ensure" == 'n' ]; then
  echo "\033[31mUser cancelled!\033[0m"
  exit
fi

if [ "$user_ensure" != 'Y' ]; then
  echo "\033[31mInvalid input!\033[0m"
  exit
fi

#Deal tag && code
echo "\nFetching tags from remote..."
git fetch --tag
tags=$(git tag)
match=$(echo "${tags}" |grep $ver)
if [ ! -z "$match" ]; then
  echo "\033[31mTag($match) is already exist!\033[0m"
  exit 0
fi

echo "Try to push tag($ver) && code"
git tag $ver
git push --tag
git push

#Deal repo
podname=$(expr "$filename" : ".*/\(.*\)\.podspec")
echo $podname

repoinfo=$(pod search $podname)
reponame=$(expr "$repoinfo" : ".*\[\(.*\) repo\]")
echo $reponame
pod repo push $reponame $filename --allow-warnings --silent
