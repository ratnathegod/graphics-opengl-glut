class Triangles:
    @staticmethod
    def isIsoscelesTriangle(x1, y1, x2, y2, x3, y3):
        def d2(ax, ay, bx, by):
            dx, dy = ax - bx, ay - by
            return dx * dx + dy * dy

        ab2 = d2(x1, y1, x2, y2)
        bc2 = d2(x2, y2, x3, y3)
        ca2 = d2(x3, y3, x1, y1)

        if ab2 == 0 or bc2 == 0 or ca2 == 0:
            return False

        twice_area = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)
        if twice_area == 0:
            return False

        return ab2 == bc2 or bc2 == ca2 or ca2 == ab2
