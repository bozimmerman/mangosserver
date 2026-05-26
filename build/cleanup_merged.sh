#!/bin/bash

# 1. Update origin to make sure we're comparing against the latest upstream
echo "Fetching latest from origin..."
git fetch origin

# 2. Identify all local branches except master
branches=$(git branch --format='%(refname:short)' | grep -v 'master')

for branch in $branches; do
    echo "Checking: $branch"

    # 3. Check if the branch is fully merged into origin/master
    # 'git cherry' compares the changes. If it returns nothing (or only lines starting with '-'), 
    # it means all commits in $branch are already in origin/master.
    MERGED_COMMITS=$(git cherry origin/master "$branch" | grep "^+")

    if [ -z "$MERGED_COMMITS" ]; then
        echo "✅ MATCH: All commits from [$branch] are already in origin/master."
        
        # 4. Prompt for deletion
        read -p "Delete local and remote (myfork) branch '$branch'? (y/n): " confirm
        if [[ "$confirm" == "y" || "$confirm" == "Y" ]]; then
            # Delete local
            git branch -D "$branch"
            # Delete from your fork
            git push myfork --delete "$branch"
        fi
    else
        echo "ℹ️  SKIPPED: [$branch] has unique commits not yet in origin/master."
    fi
    echo "----------------------------------------------------"
done

