#!/bin/bash
if [ -d "BeyondYourComprehensionFHE" ]; then
    echo "⚠️ Repo exists! Updating..."
    cd BeyondYourComprehensionFHE
    git pull
    cd ..
else
    echo "📥 Cloning B6 HYDRA..."
    git clone https://github.com/primordialomegazero/BeyondYourComprehensionFHE.git
fi
