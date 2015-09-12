from judge import Result

class BadLength(BaseException):
    pass
class WrongAnswer(BaseException):
    pass
class Interactor(object):
    def __init__(self, process, input_file, output_file, max_points):
        self.process = process
        self.input_file = input_file
        self.output_file = output_file
        self.max_points = max_points
        self.result = Result()
        self.result.result_flag = Result.AC
        self.result.proc_output = []
        self.result_points_set = False
        self.finalized = False
    def write(self, s):
        try:
            self.process.stdin.write(s)
            self.process.stdin.flush()
        except: # probably broken pipe
            pass
    def writeln(self, s):
        self.write(s + '\n')
    def println(self, s):
        self.writeln(str(s))
    def next_line(self, file_handle):
        if file_handle:
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
    def next_int_input(self, lo=float('-inf'), hi=float('inf')):
        return self.next_int(self.input_file, lo, hi)
    def next_int_output(self, lo=float('-inf'), hi=float('inf')):
        return self.next_int(self.output_file, lo, hi)
    def next_int_progout(self, lo=float('-inf'), hi=float('inf')):
        return self.next_int(self.process.stdout, lo, hi)
    def wrong_answer(self):
        self.result.result_flag = Result.WA
        self.finalize()
        raise WrongAnswer()
    def set_points(self, numerator=1, denominator=1):
        if numerator < 0:
            numerator = 0
        if numerator >= denominator:
            self.result.points = self.max_points
        else:
            self.result.points = numerator * self.max_points / denominator
        self.result_points_set = True
    def finalize(self):
        if not self.finalized:
            while True:
                line = self.process.stdout.readline()
                if line == "":
                    break
                line = line.rstrip()
                if line != "":
                    self.result.proc_output.append(line)
            self.result.proc_output = "".join(self.result.proc_output)
            if self.result.result_flag == Result.AC and not self.result_points_set:
                self.set_points()
            self.finalized = True
    def get_result(self):
        self.finalize()
        return self.result
def grade(case, process, case_input=None, case_output=None, point_value="0", **kwargs):
    I = Interactor(process, case_input, case_output, int(point_value))
    try:
        wrong_guesses = 0
        N = I.next_int_input()
        I.println(N)
        piles = []
        xor_all = 0
        sum_all = 0
        for _ in xrange(N):
            x = I.next_int_input()
            piles.append(x)
            I.println(x)
            xor_all ^= x
            sum_all += x
        turn = 1
        while True:
            pile, amount = map(int, I.next_line_progout().split())
            if pile < 1 or pile > N:
                I.wrong_answer()
            pile -= 1
            if amount < 0 or (amount == 0 and turn != 1) or amount > piles[pile]:
                I.wrong_answer()
            xor_all ^= piles[pile]
            piles[pile] -= amount
            xor_all ^= piles[pile]
            sum_all -= amount
            # maxi = (need to remove, original size, index)
            maxi = max((0, 0, N + 1), max((piles[i] - (xor_all ^ piles[i]), piles[i], i) for i in xrange(N)))
            if maxi[2] == N + 1:
                I.println("0 0")
                I.set_points()
                break
            elif maxi[0] == 0:
                I.println("%d %d" % (maxi[2] + 1, maxi[1]))
                xor_all ^= maxi[1]
                sum_all -= maxi[1]
                piles[maxi[2]] = 0
            else:
                I.println("%d %d" % (maxi[2] + 1, maxi[0]))
                xor_all ^= piles[maxi[2]]
                piles[maxi[2]] -= maxi[0]
                xor_all ^= piles[maxi[2]]
                sum_all -= maxi[0]
            if sum_all == 0:
                I.result_flag = Result.WA
                I.set_points(0)
                break
            turn += 1
    except WrongAnswer:
        pass
    except:
        I.result_flag = Result.WA
        I.set_points(0)
    return I.get_result()
