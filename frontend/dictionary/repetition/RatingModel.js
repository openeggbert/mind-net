export class RatingModel {
    selected_grade = -1
    rating_buttons = []
    answer_change_count = -1
    static grades = Array.from([0, 1, 2, 3, 4, 5])
    reset() {
        this.selected_grade = -1
        this.rating_buttons = []
        this.answer_change_count = 0
    }
}