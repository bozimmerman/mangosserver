#!/bin/bash
# Check if we're in a git repository
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    echo "Error: Not in a git repository"
    exit 1
fi
# Check if we're on master
current_branch=$(git branch --show-current)
if [ "$current_branch" != "master" ]; then
    echo "Error: Not on master branch (currently on $current_branch)"
    exit 1
fi
# Ask if they've staged their changes
read -p "Have you already staged all your changes with 'git add'? (y/n): " staged
if [ "$staged" != "y" ] && [ "$staged" != "Y" ]; then
	exit 1
fi
# Check if there are staged changes
if git diff --cached --quiet; then
    echo "Error: No changes staged for commit"
    exit 1
fi
# Get commit message
read -p "Enter commit message: " commit_msg
if [ -z "$commit_msg" ]; then
    echo "Error: Commit message cannot be empty"
    exit 1
fi
# Get branch name
read -p "Enter new branch name (will be based on origin/master): " branch_name
if [ -z "$branch_name" ]; then
    echo "Error: Branch name cannot be empty"
    exit 1
fi
# Check if branch already exists
if git show-ref --verify --quiet "refs/heads/$branch_name"; then
    echo "Error: Branch '$branch_name' already exists"
    exit 1
fi
# Commit to master first
if ! git commit -m "$commit_msg"; then
    echo "Error: Commit to master failed"
    exit 1
fi
echo "Committed to master"

# Save the commit hash we just created
commit_hash=$(git rev-parse HEAD)

# Stash any uncommitted changes (like modifications to this script)
stash_needed=false
if ! git diff --quiet || ! git diff --cached --quiet; then
    echo "Stashing uncommitted changes..."
    git stash
    stash_needed=true
fi

# Create new branch from origin/master
if ! git checkout -b "$branch_name" origin/master; then
    echo "Error: Failed to create branch from origin/master"
    if [ "$stash_needed" = true ]; then
        git checkout master
        git stash pop
    fi
    exit 1
fi

# Cherry-pick the specific commit we saved
if ! git cherry-pick "$commit_hash"; then
    echo "Error: Failed to cherry-pick commit to new branch"
    echo "You may have conflicts. Resolve them, then run:"
    echo "  git cherry-pick --continue"
    echo "  git push myfork $branch_name"
    echo "  git checkout master"
    if [ "$stash_needed" = true ]; then
        echo "  git stash pop"
    fi
    exit 1
fi

# Push the new branch to myfork for PR
echo "Pushing new branch '$branch_name' to myfork..."
if ! git push myfork "$branch_name"; then
    echo "Warning: Failed to push branch to myfork"
    echo "Your commit is on local branch '$branch_name'"
    git checkout master
    if [ "$stash_needed" = true ]; then
        git stash pop
    fi
    exit 1
fi

# Switch back to master
git checkout master

# Restore stashed changes if any
if [ "$stash_needed" = true ]; then
    echo "Restoring stashed changes..."
    git stash pop
fi

# Push updated master to myfork
echo "Pushing master to myfork..."
if ! git push myfork master; then
    echo "Warning: Failed to push master to myfork"
    exit 1
fi

echo "Success!"
echo "- Committed to master and pushed to myfork/master"
echo "- Created branch '$branch_name' from origin/master with the same commit"
echo "- Pushed '$branch_name' to myfork (ready for PR)"

