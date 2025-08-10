# MINISHELL v2.0 - Comprehensive Testing Guide

## Overview
This guide provides comprehensive testing procedures for the improved MINISHELL with enhanced security, memory management, and error handling.

## Pre-Testing Setup

1. **Build the shell:**
   ```bash
   make clean && make
   ```

2. **Verify external commands file exists:**
   ```bash
   ls -la external_commands.txt
   ```

## Test Categories

### 1. Basic Functionality Tests

#### 1.1 Shell Startup
```bash
./shell
```
**Expected:** 
- Clear screen
- Display version information
- Show command count loaded
- Display prompt: `[COMMAND]$`

#### 1.2 Prompt Customization
```bash
# Test valid PS1 change
PS1=MyShell

# Test invalid PS1 (should show error)
PS1=

# Test PS1 with spaces (should show error)
PS1= 

# Test PS1 back to default
PS1=COMMAND
```

#### 1.3 Empty Input Handling
```bash
# Press Enter with no input - should show prompt again
<ENTER>

# Test with just spaces
    <ENTER>

# Test with tabs
	<ENTER>
```

### 2. Builtin Commands Testing

#### 2.1 Echo Commands
```bash
# Basic echo
echo Hello World

# Echo exit status (should be 0 initially)
echo $?

# Echo process ID
echo $$

# Echo shell path (current directory)
echo $SHELL

# Empty echo
echo
```

#### 2.2 Directory Commands
```bash
# Show current directory
pwd

# Change to home directory
cd

# Change to specific directory
cd /tmp
pwd

# Change to non-existent directory (should show error)
cd /nonexistent

# Change to parent directory
cd ..
pwd

# Invalid cd usage
cd invalid extra args
```

#### 2.3 Exit Command
```bash
# Exit with default code
exit

# Exit with specific code (start new shell first)
exit 0
exit 1
exit 255

# Invalid exit codes
exit abc
exit -1
exit 256
```

### 3. External Commands Testing

#### 3.1 Basic External Commands
```bash
# Test various external commands
ls
ls -la
cat /etc/passwd
date
uname -a
ps aux
```

#### 3.2 Command with Arguments
```bash
# Commands with multiple arguments
ls -la /tmp
grep root /etc/passwd
find . -name "*.c"
wc -l *.c
```

#### 3.3 Non-existent Commands
```bash
# Should show "Command not found" error
nonexistentcommand
invalid_program
```

### 4. Signal Handling Tests

#### 4.1 Interrupt Handling
```bash
# Start a long-running command then press Ctrl+C
sleep 10
# Press Ctrl+C - should interrupt and return to prompt

# Try Ctrl+C at prompt - should display message and new prompt
# Press Ctrl+C
```

#### 4.2 Process Termination
```bash
# Test external process termination
sleep 5 &
# Wait for completion - should show termination message
```

### 5. Security and Error Handling Tests

#### 5.1 Buffer Overflow Prevention
```bash
# Test very long input (should be safely truncated)
echo ThisIsAVeryLongCommandWithLotsOfArgumentsThatShouldTestBufferLimitsAndMakeSureNoOverflowOccursInTheImprovedVersionOfMinishellWithSecurityEnhancements

# Test long command names
ThisIsAnExtremelyLongCommandNameThatShouldBeHandledSafelyWithoutCausingBufferOverflowsOrSecurityIssues
```

#### 5.2 Input Validation
```bash
# Test special characters
echo "Hello World"
echo 'Single quotes'
echo $HOME
echo $(whoami)

# Test edge cases
cd ~
cd $HOME
```

#### 5.3 Memory Management
```bash
# These tests verify no memory leaks occur
# Run multiple commands to test memory cleanup
for i in {1..100}; do echo "Test $i"; done

# Test external commands repeatedly
ls
ps
date
pwd
ls
ps
date
pwd
```

### 6. File System Tests

#### 6.1 File Operations
```bash
# Test file operations via external commands
touch testfile.txt
ls -la testfile.txt
cat testfile.txt
rm testfile.txt
ls -la testfile.txt
```

#### 6.2 Directory Operations
```bash
mkdir testdir
cd testdir
pwd
touch file1.txt file2.txt
ls -la
cd ..
rmdir testdir
```

### 7. Edge Cases and Stress Tests

#### 7.1 Rapid Input
```bash
# Quickly type multiple commands
pwd<ENTER>ls<ENTER>date<ENTER>echo test<ENTER>
```

#### 7.2 Mixed Command Types
```bash
# Mix builtin and external commands
pwd
ls
echo "Current directory:"
pwd
date
echo $?
cd /tmp
ls
pwd
cd
```

#### 7.3 EOF Handling
```bash
# Test Ctrl+D (EOF) - should exit gracefully
# Press Ctrl+D
```

### 8. Performance Tests

#### 8.1 Command Loading
```bash
# Verify external commands load correctly at startup
# Check startup message for command count
```

#### 8.2 Response Time
```bash
# Test responsive command execution
echo start
date
echo end
```

## Automated Test Script

Create and run this test script:

```bash
#!/bin/bash
# automated_test.sh

echo "=== MINISHELL Automated Tests ==="

# Test basic commands
echo "pwd" | timeout 5 ./shell
echo "date" | timeout 5 ./shell  
echo "echo Hello" | timeout 5 ./shell
echo "echo \$?" | timeout 5 ./shell
echo "PS1=TEST" | timeout 5 ./shell

echo "=== All basic tests completed ==="
```

## Expected Behaviors

### ✅ Success Indicators
- No segmentation faults
- No memory leaks
- Proper error messages
- Clean exit on EOF or exit command
- Correct prompt display
- External commands execute properly
- Builtin commands work as expected

### ❌ Failure Indicators
- Crashes or segfaults
- Infinite loops
- Unresponsive shell
- Memory leaks
- Buffer overflows
- Incorrect exit codes
- Malformed output

## Performance Benchmarks

### Memory Usage
- Baseline memory: < 2MB
- After 100 commands: < 5MB growth
- No memory leaks after exit

### Response Time
- Command execution: < 1 second
- External command loading: < 2 seconds
- Prompt display: Immediate

## Security Verification

1. **Buffer Safety**: All input functions use safe bounds checking
2. **Command Injection**: No unsafe `system()` calls
3. **Memory Safety**: Proper allocation and cleanup
4. **Signal Safety**: Graceful signal handling
5. **Input Validation**: All user input is validated

## Troubleshooting

### Common Issues
1. **External commands not found**: Check `external_commands.txt` exists
2. **Permission denied**: Ensure shell has execute permissions
3. **Compilation errors**: Verify all source files are present

### Debug Mode
```bash
# Run with debug info
valgrind --leak-check=full ./shell

# Check for memory errors
valgrind --tool=memcheck ./shell
```

## Test Results Documentation

Create a test log:
```bash
# Document your test results
echo "Test Date: $(date)" > test_results.log
echo "Tester: [Your Name]" >> test_results.log
echo "Version: MINISHELL v2.0" >> test_results.log
echo "Results:" >> test_results.log
```

Record pass/fail status for each test category and any issues encountered.