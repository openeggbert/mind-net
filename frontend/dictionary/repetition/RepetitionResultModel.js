import {ReviewResult} from "./ReviewResult.js";
import {defined} from "../../common.js";

export class RepetitionResultModel {
    #total = 0
    #done = 0

    #skipped = 0
    #errors = 0
    #not_due_yet = 0
    #reviewed = 0
    #unknown = 0

    #reviews = []
    #started_at = Date.now()
    #ended_at = null
    constructor(searches) {
        this.#total = searches.length
    }
    next_done(review_result, review) {
        this.#done++
        if(review) this.#reviews.push(review)
        switch(review_result.label) {
            case ReviewResult.Reviewed.label: this.#reviewed++; break;
            case ReviewResult.Error.label: this.#errors++; break;
            case ReviewResult.Skipped.label: this.#skipped++; break;
            case ReviewResult.NotDueYet.label: this.#not_due_yet++; break;
            default: {
                this.#unknown++;
                console.warn("Unknown review_result: " + JSON.stringify(review_result))
            }
        }
    }
    end() {
        if(defined(this.#ended_at)) return
        this.#ended_at = Date.now()
    }
    get_total_count() {return this.#total}
    get_done_count() {return this.#done}
    get_count(review_result) {
        switch(review_result.label) {
            case ReviewResult.Reviewed.label: return this.#reviewed
            case ReviewResult.Error.label: return this.#errors
            case ReviewResult.Skipped.label: return this.#skipped
            case ReviewResult.NotDueYet.label: return this.#not_due_yet
            case ReviewResult.Unknown.label: return this.#unknown
            default: {
                console.warn("Unknown review_result: " + JSON.stringify(review_result))
                return 0
            }
        }
    }
    get_started_at() {
        return this.#started_at
    }
    get_ended_at() {
        return this.#ended_at
    }
    #formatDuration(ms) {
        if (typeof ms !== "number" || ms < 0) {
            return "00:00:00";
        }

        const totalSeconds = Math.floor(ms / 1000);

        const hours   = Math.floor(totalSeconds / 3600);
        const minutes = Math.floor((totalSeconds % 3600) / 60);
        const seconds = totalSeconds % 60;

        const pad = n => String(n).padStart(2, "0");

        return `${pad(hours)}:${pad(minutes)}:${pad(seconds)}`;
    }

    get_duration() {
        return this.#formatDuration((defined(this.#ended_at) ?this.#ended_at : Date.now()) - this.#started_at)
    }
    // get_reviews() {
    //     return this.#reviews
    // }
    get_average_latency_s() {
        if (this.#reviews.length === 0) return 0

        const total = this.#reviews.reduce(
            (sum, r) => sum + (typeof r.latency_ms === "number" ? r.latency_ms : 0),
            0
        )

        return Math.round(total / this.#reviews.length) / 1000.0
    }
    get_min_latency_s() {
        if (this.#reviews.length === 0) return 0

        let min = Number.POSITIVE_INFINITY

        for (const r of this.#reviews) {
            if (typeof r.latency_ms === "number" && r.latency_ms < min) {
                min = r.latency_ms
            }
        }

        return min === Number.POSITIVE_INFINITY ? 0 : min / 1000.0
    }
    get_max_latency_s() {
        if (this.#reviews.length === 0) return 0

        let max = 0

        for (const r of this.#reviews) {
            if (typeof r.latency_ms === "number" && r.latency_ms > max) {
                max = r.latency_ms
            }
        }

        return max / 1000.0
    }

    get_total_latency_s() {
        return this.#reviews.reduce(
            (sum, r) => sum + (typeof r.latency_ms === "number" ? r.latency_ms : 0),
            0
        ) / 1000.0
    }

    get_average_grade() {
        const grades = this.#reviews
            .map(r => r.grade)
            .filter(g => typeof g === "number")

        if (grades.length === 0) return 0

        const sum = grades.reduce((a, b) => a + b, 0)
        return sum / grades.length
    }

    get_total_answer_change_count() {
        return this.#reviews.reduce(
            (sum, r) => sum + (typeof r.answer_change_count === "number" ? r.answer_change_count : 0),
            0
        )
    }


}