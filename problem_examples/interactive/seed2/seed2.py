from dmoj.graders.standard import StandardGrader
    
class Grader(StandardGrader):
    def _interact_with_process(self, case, result, input):
        process = self._current_proc

        try:
            guesses = 0
            guess = 0
            N = int(case.input_data())
            while guess != N:
                guess = int(process.stdout.readline())
                
                assert 1 <= guess <= 2000000000
                
                guesses += 1
                if guess == N:
                    message = "OK"
                elif guess > N:
                    message =  "FLOATS"
                else:
                    message = "SINKS"
                process.stdin.write(message + '\n')
                process.stdin.flush()

            for stream in [process.stdin, process.stdout, process.stderr]:
                stream.close()

            self.check = guesses <= 31
        except:
            self.check = False

        process.wait()

    def check_result(self, case, result):
        return self.check
