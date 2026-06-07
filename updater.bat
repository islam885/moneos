@echo off
echo git update files

git add .

git commit -m "update" 

git push origin main --force

echo.
echo DONE!
pause