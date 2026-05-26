git fetch origin
git checkout master
git merge origin/master
echo if not up-to-date, do git push myfork master
echo then do git fetch myfork
echo then do git merge myfork/master
