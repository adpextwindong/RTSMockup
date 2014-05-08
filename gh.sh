giturl=$(git config --get remote.origin.url)
start $(echo ${giturl/.git/})