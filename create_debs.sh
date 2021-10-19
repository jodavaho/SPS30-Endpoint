#!/bin/bash
version="1.0"
# Exit on error
set -e
git checkout $version
rmdir fakeroot -rf

