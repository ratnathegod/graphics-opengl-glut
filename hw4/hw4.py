import math

class Fibonacci:
    def isFibonacciNumber(self, n):
        """
        Return True if n is a Fibonacci number, False otherwise.
        Uses the number theory fact:
          n is Fibonacci <=> 5*n*n + 4 or 5*n*n - 4 is a perfect square.
        """
        if n < 0:
            return False
        if n == 0 or n == 1:
            return True

        def is_perfect_square(x: int) -> bool:
            if x < 0:
                return False
            r = math.isqrt(x)  
            return r * r == x

        return (is_perfect_square(5*n*n + 4) or
                is_perfect_square(5*n*n - 4))
