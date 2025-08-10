#!/bin/bash

# MINISHELL v2.0 Automated Test Suite
# Run comprehensive tests on the improved minishell

echo "========================================"
echo "MINISHELL v2.0 - Automated Test Suite"
echo "========================================"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Test result function
test_result() {
    local test_name="$1"
    local result="$2"
    local expected="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✓${NC} $test_name: PASSED"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗${NC} $test_name: FAILED (expected: $expected, got: $result)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# Function to run shell command with timeout
run_shell_test() {
    local cmd="$1"
    local timeout_val="${2:-5}"
    
    echo "$cmd" | timeout "$timeout_val" ./shell 2>/dev/null
    return $?
}

echo -e "${BLUE}Building minishell...${NC}"
make clean >/dev/null 2>&1
if make >/dev/null 2>&1; then
    echo -e "${GREEN}✓${NC} Build successful"
else
    echo -e "${RED}✗${NC} Build failed"
    exit 1
fi

echo ""
echo -e "${BLUE}Running Basic Functionality Tests...${NC}"

# Test 1: Shell starts and exits
echo "exit 0" | timeout 5 ./shell >/dev/null 2>&1
test_result "Shell startup and exit" "$?" "0"

# Test 2: PWD command
PWD_OUTPUT=$(printf 'pwd\nexit\n' | timeout 5 ./shell 2>/dev/null | grep "$(pwd)")
if [[ -n "$PWD_OUTPUT" ]]; then
    test_result "PWD command" "0" "0"
else
    test_result "PWD command" "1" "0"
fi

# Test 3: Echo command
ECHO_OUTPUT=$(echo "echo Hello" | timeout 5 ./shell 2>/dev/null | grep "Hello")
if [[ "$ECHO_OUTPUT" == *"Hello"* ]]; then
    test_result "Echo command" "0" "0"
else
    test_result "Echo command" "1" "0"
fi

# Test 4: Echo $$ (PID)
PID_TEST_OUTPUT=$(printf 'echo $$\nexit\n' | timeout 5 ./shell 2>&1)
if echo "$PID_TEST_OUTPUT" | grep -q '[0-9]\{4,\}'; then
    test_result "Echo PID command" "0" "0"
else
    test_result "Echo PID command" "1" "0"
fi

# Test 5: Echo $? (exit status)  
STATUS_TEST_OUTPUT=$(printf 'echo $?\nexit\n' | timeout 5 ./shell 2>&1)
if echo "$STATUS_TEST_OUTPUT" | grep -q 'COMMAND.*0$'; then
    test_result "Echo exit status command" "0" "0"
else
    test_result "Echo exit status command" "1" "0"
fi

echo ""
echo -e "${BLUE}Running External Command Tests...${NC}"

# Test 6: LS command
LS_OUTPUT=$(echo "ls" | timeout 5 ./shell 2>/dev/null | wc -l)
if [[ "$LS_OUTPUT" -gt 0 ]]; then
    test_result "LS external command" "0" "0"
else
    test_result "LS external command" "1" "0"
fi

# Test 7: Date command
DATE_OUTPUT=$(echo "date" | timeout 5 ./shell 2>/dev/null | grep -E '[0-9]{4}')
if [[ -n "$DATE_OUTPUT" ]]; then
    test_result "Date external command" "0" "0"
else
    test_result "Date external command" "1" "0"
fi

echo ""
echo -e "${BLUE}Running Error Handling Tests...${NC}"

# Test 8: Invalid command
INVALID_OUTPUT=$(echo "nonexistentcommand123" | timeout 5 ./shell 2>/dev/null | grep -i "not found")
if [[ -n "$INVALID_OUTPUT" ]]; then
    test_result "Invalid command handling" "0" "0"
else
    test_result "Invalid command handling" "1" "0"
fi

# Test 9: Empty input handling
echo "" | timeout 5 ./shell >/dev/null 2>&1
test_result "Empty input handling" "$?" "0"

echo ""
echo -e "${BLUE}Running PS1 Prompt Tests...${NC}"

# Test 10: PS1 change
PS1_OUTPUT=$(echo -e "PS1=TEST\nexit" | timeout 5 ./shell 2>/dev/null)
if [[ "$PS1_OUTPUT" == *"TEST"* ]]; then
    test_result "PS1 prompt change" "0" "0"
else
    test_result "PS1 prompt change" "1" "0"
fi

echo ""
echo -e "${BLUE}Running Memory Safety Tests...${NC}"

# Test 11: Long input handling (buffer overflow prevention)
LONG_INPUT=$(printf 'a%.0s' {1..2000})
echo "echo $LONG_INPUT" | timeout 5 ./shell >/dev/null 2>&1
test_result "Long input handling" "$?" "0"

# Test 12: Multiple rapid commands
{
    echo "pwd"
    echo "echo test1"
    echo "echo test2" 
    echo "echo test3"
    echo "date"
    echo "ls"
    echo "exit"
} | timeout 10 ./shell >/dev/null 2>&1
test_result "Multiple rapid commands" "$?" "0"

echo ""
echo -e "${BLUE}Running File System Tests...${NC}"

# Test 13: CD command functionality
{
    echo "cd /tmp"
    echo "pwd"
    echo "cd"
    echo "exit"
} | timeout 5 ./shell 2>/dev/null | grep -q "/tmp"
test_result "CD command functionality" "$?" "0"

echo ""
echo -e "${BLUE}Running Memory Leak Tests...${NC}"

# Test 14: Valgrind memory check (if available)
if command -v valgrind >/dev/null 2>&1; then
    VALGRIND_OUTPUT=$(echo -e "pwd\necho test\nls\nexit" | timeout 10 valgrind --leak-check=summary ./shell 2>&1)
    if echo "$VALGRIND_OUTPUT" | grep -q "no leaks are possible"; then
        test_result "Memory leak check" "0" "0"
    else
        test_result "Memory leak check" "1" "0"
    fi
else
    echo -e "${YELLOW}⚠${NC} Valgrind not available, skipping memory leak test"
fi

# Test 15: External commands file loading
if [[ -f "external_commands.txt" ]]; then
    LOAD_OUTPUT=$(echo "exit" | timeout 5 ./shell 2>&1 | grep -i "loaded.*commands")
    if [[ -n "$LOAD_OUTPUT" ]]; then
        test_result "External commands loading" "0" "0"
    else
        test_result "External commands loading" "1" "0"
    fi
else
    test_result "External commands file exists" "1" "0"
fi

echo ""
echo "========================================"
echo -e "${BLUE}Test Summary${NC}"
echo "========================================"
echo "Total Tests: $TOTAL_TESTS"
echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed: ${RED}$FAILED_TESTS${NC}"

if [ "$FAILED_TESTS" -eq 0 ]; then
    echo -e "${GREEN}🎉 All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}⚠ $FAILED_TESTS test(s) failed${NC}"
    exit 1
fi