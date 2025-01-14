package test

import (
	"fmt"
	"os/exec"
	"testing"
)

const (
	GOOD = false
	BAD  = true
)

type Test struct {
	DesiredResult bool
	FileToTest    string
}

func runTestParser(file string) (int, error) {
	cmd := exec.Command("../../webserv", file)
	output, err := cmd.CombinedOutput()
	if err != nil {
		return cmd.ProcessState.ExitCode(), fmt.Errorf("Error ejecutando el programa C++: %v, output %s", err, output)
	}

	return cmd.ProcessState.ExitCode(), nil
}

func testGoodExec(file string, t *testing.T) bool {
	exitCode, err := runTestParser(file)
	if exitCode == 0 && err == nil {
		t.Logf("FILE PASS: DEXIT: 0 || EXIT CODE :%d\n", exitCode)
		return true
	}
	t.Logf("FILE FAILED: DEXIT: 0 || EXIT CODE :%d, ERR:%s\n", exitCode, err)
	return false
}

func testBadExec(file string, t *testing.T) bool {
	exitCode, err := runTestParser(file)
	if err != nil || exitCode == 1 {
		t.Logf("FILE PASS: DEXIT: 1, DERROR || EXIT CODE :%d, ERR:%s\n", exitCode, err)
		return true
	}
	t.Logf("FILE FAILED: DEXIT: 1, DERROR || EXIT CODE :%d, ERR:%s\n", exitCode, err)
	return false
}

func TestParser(t *testing.T) {
	tests := []Test{
		{DesiredResult: BAD, FileToTest: "/"},
		{DesiredResult: GOOD, FileToTest: "./../nginx/conf/1.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/2.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/3.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/4.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/nginx0.conf"},
		{DesiredResult: GOOD, FileToTest: "./../nginx/conf/nginx1.conf"},
		{DesiredResult: GOOD, FileToTest: "./../nginx/conf/nginx2.conf"},
		{DesiredResult: GOOD, FileToTest: "./../nginx/conf/nginx3.conf"},
		{DesiredResult: GOOD, FileToTest: "./../nginx/conf/nginx4.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/nginx5.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/nginx6.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/nginx7.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/nginx7.conf"},
		{DesiredResult: GOOD, FileToTest: "./../nginx/conf/err1.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/err2.conf"},
		{DesiredResult: GOOD, FileToTest: "./../nginx/conf/err3.conf"},
		{DesiredResult: GOOD, FileToTest: "./../nginx/conf/err4.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/err5.conf"},
		{DesiredResult: GOOD, FileToTest: "./../nginx/conf/err6.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/err7.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/errmaybe1.conf"},
		{DesiredResult: BAD, FileToTest: "./../nginx/conf/errmaybe2.conf"},
	}

	for _, test := range tests {
		if test.DesiredResult == GOOD {
			if !testGoodExec(test.FileToTest, t) {
				t.Errorf("La prueba exitosa para el archivo %s falló", test.FileToTest)
			}
		} else {
			if !testBadExec(test.FileToTest, t) {
				t.Errorf("La prueba fallida para el archivo %s debería haber fallado pero tuvo éxito", test.FileToTest)
			}
		}
	}
}
