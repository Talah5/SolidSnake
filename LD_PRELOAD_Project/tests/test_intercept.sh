#!/bin/bash
# Script pour tester LD_PRELOAD
echo "Test de LD_PRELOAD avec exfiltration"
LD_PRELOAD=../build/intercept.so cat /etc/passwd

