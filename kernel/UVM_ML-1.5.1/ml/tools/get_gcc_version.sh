#!/bin/bash
${1:-g++} --version | grep -E '[0-9]+\.[0-9]+\.[0-9]+( |$)' -o | grep -E '[0-9]+\.[0-9]+' -o