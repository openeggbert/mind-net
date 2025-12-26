export class AskForGradeModel {
    constructor(grade = -1, answer_change_count = 0) {
        this.grade = grade
        this.answer_change_count = answer_change_count < 0 ? 0 : answer_change_count
    }

    to_json() {
        return {
            grade: this.grade,
            answer_change_count: this.answer_change_count,
        }
    }
}