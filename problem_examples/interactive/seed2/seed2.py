import platform

from dmoj.result import Result, CheckerResult
from dmoj.graders.standard import StandardGrader
# What gets displayed after judging
# ie. time, mem, AC/WA/IR etc.

class BadLength(BaseException):
    pass
class WrongAnswer(BaseException):
    pass
class Interactor(object):
    def __init__(self, case, process, result):
        self.process = process
        self.case = case
        self.input_file = case.input_data()
        self.output_file = case.output_data()
        self.max_points = case.points
        self.result = result
        self.result.result_flag = Result.AC
        self.result.proc_output = []
        self.result_points_set = False
        self.finalized = False
    def write(self, s):
        try:
            self.process.stdin.write(s)
            self.process.stdin.flush()
        except: # probably broken pipe
            print "The pipe broke (maybe)!"
            pass
    def writeln(self, s):
        self.write(s + '\n')
    def println(self, s):
        self.writeln(str(s))
    def next_line(self, file_handle):
        if self.process.stdout:
            try:
                while True:
                    ret = file_handle.readline()
                    if ret == "":
                        raise BadLength()
                    if file_handle == self.process.stdout:
                        self.result.proc_output.append(ret)
                    ret = ret.rstrip()
                    if ret != "":
                        return ret
            except BadLength:
                self.wrong_answer()
            except:
                self.wrong_answer()
                raise
        return ""
    def next_line_input(self):
        return self.next_line(self.input_file)
    def next_line_output(self):
        # Should probably be case.output() or something
        return self.next_line(self.output_file)
    def next_line_progout(self):
        return self.next_line(self.process.stdout)
    def next_int(self, file_handle, lo, hi):
        try:
            ret = int(self.next_line(file_handle))
        except:
            self.wrong_answer()
        if ret < lo or ret > hi:
            self.wrong_answer()
        return ret
    def next_int_progout(self, lo=float('-inf'), hi=float('inf')):
        return self.next_int(self.process.stdout, lo, hi)
    def wrong_answer(self):
        # Gotta terminate somehow...
        self.result.result_flag = Result.WA
        self.finalize()
        raise WrongAnswer()
    def finalize(self):
        if not self.finalized:
            self.result.proc_output = filter(None, self.result.proc_output)
            self.result.proc_output = ''.join(self.result.proc_output) or ''
            self.finalized = True
    def get_result(self):
        self.finalize()
        return self.result
        
    def close(self):
         self.finalize()
         self.process.stdin.close()
         self.process.stdout.close()
         self.process.stderr.close()
         self.process.wait()
        
class Grader(StandardGrader):
    def grade(self, case):
        result = Result(case)
        self._current_proc = self.binary.launch(time=self.problem.time_limit, memory=self.problem.memory_limit,
                                                pipe_stderr=True, unbuffered=case.config.unbuffered,
                                                io_redirects=case.io_redirects())
        process = self._current_proc
        I = Interactor(case, process, result)
        # process is a process object ie. stdin, stdout, stderr
        # case_input is the data given, the .in file
        check = False
        try:
            guesses = 0
            N = int(case.input_data())
            # N = int(I.next_line_input())
            while True:
                guess = I.next_int_progout(1, 2000000000)
                guesses += 1
                if guess == N:
                    I.println("OK")
                    break
                elif guess > N:
                    I.println("FLOATS")
                else:
                    I.println("SINKS")
            I.close()
            if guesses > 31:
                check = False
            else:
                check = True
        except WrongAnswer:
            check = False
        self._current_proc.wait()
        process = self._current_proc
        result.max_memory = process.max_memory or 0.0
        result.execution_time = process.execution_time or 0.0
        result.r_execution_time = process.r_execution_time or 0.0
        self.set_result_flag(process, result)
        if not result.result_flag:
            check = CheckerResult(check, case.points if check else 0)
        else:
            check = CheckerResult(False, 0)
        result.result_flag |= [Result.WA, Result.AC][check.passed]
        result.points = check.points
        try:
            error = process.stderr.read() or ''
        except ValueError:
            error = ''
        self.update_feedback(check, error, process, result)
        case.free_data()
        if platform.python_implementation() == 'PyPy':
            gc.collect()
        return result
