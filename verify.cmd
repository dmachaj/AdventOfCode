@echo off
Powershell -ExecutionPolicy bypass %~dp0\verify.ps1 -SubDir %1
