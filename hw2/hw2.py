class Students:
    def get_students_taking_at_least_one_course(self, students_courses, target_courses):
        """
        Return a sorted list of unique student names who are enrolled in
        at least one course from target_courses.

        students_courses: list[tuple[str, str]]
        target_courses:   list[str]
        """
        targets = set(target_courses)
        names = set()
        for name, course in students_courses:
            if course in targets:
                names.add(name)
        return sorted(names)