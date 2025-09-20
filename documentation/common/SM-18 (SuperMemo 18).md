# SM-18 (SuperMemo 18)

User defines the quality of the knowledge (0-5).

Input values:
- Quality (0-5)
- Repetition - how many times the note was successfully reviewed
    - Starts at 0
    - Increases with each successful repetition (quality ≥ 3)
    - Helps determine if the interval should be extended
- Interval - current interval in days
    - Number of days until next repetition
    - E.g. 1 → 6 → 15 → 37 → 90 ...
    - Calculated based on EF and number of repetitions
- Ef - effectivity factor (initial value 2.5)
    - Starts at 2.5
    - Changes based on answer quality
    - Determines how quickly the interval extends (e.g. 10 days × EF)

Output values:
- Last quality
    - Date (timestamp) when the note should be shown again
    - Used for selecting notes for review
- Repetitions
- Interval
- Ef
- Next review
    - Date (timestamp) when the note should be shown again
    - Used for selecting notes for review
- Last review


## How to Identify Articles with Weak Knowledge

With your current SQLite schema, the application can easily detect which articles (or topics) a user is struggling with. Here are several strategies:

### Select Questions with Weak SM-2 State

Use the `state` table to find questions that have:

- Low **EF** (easiness factor)
- Few **repetitions**
- `next_review` in the past (**overdue**)

```sql
SELECT q.*
FROM question q
JOIN state s ON q.id = s.question_id
WHERE s.user_id = ?
  AND (s.EF < 2.0 OR s.repetitions < 3 OR s.next_review <= CURRENT_TIMESTAMP)
  AND q.active = 1;
````

---

### Aggregate Weak Questions by Article

If each question is linked to an `article_id`, you can group weak questions by article:

```sql
SELECT q.article_id, COUNT(*) AS weak_questions
FROM question q
JOIN state s ON q.id = s.question_id
WHERE s.user_id = ?
  AND (s.EF < 2.0 OR s.repetitions < 3 OR s.next_review <= CURRENT_TIMESTAMP)
GROUP BY q.article_id
ORDER BY weak_questions DESC;
```

This gives you a ranked list of articles with the most weak questions for a given user.

---

### Alternatively: Use `review.grade`

If you prefer to assess knowledge based on recent review performance:

```sql
SELECT q.article_id, AVG(r.grade) AS avg_grade
FROM question q
JOIN review r ON q.id = r.question_id
WHERE r.user_id = ?
GROUP BY q.article_id
HAVING avg_grade < 3
ORDER BY avg_grade ASC;
```

This highlights questions where the user consistently scores poorly.

## Component Test

### Scenario ###

- Frontend shows
    * title of the question.
    * buttons: "Show answer", "Skip"
- Once the user clicks "Show answer"
    * The application shows: How well do you understand this? (0-5):
    * User enters a number (0-5)
    * Application updates REVIEW_IN_X_DAYS, EASINESS_FACTOR, REPETITION, EXPIRES_AT
    * Continues to the next node

### Test modes ###

**Scope:** 
 - Full review - all notes
 - Due Only review - only notes due for review

**Filter:**
 - Notes under the current note
 - Date range review - notes created within a date range
 - Tags review - notes with a specific tag
 - Collection review - notes from a specific collection


## External links

 * https://en.wikipedia.org/wiki/SuperMemo
 * https://supermemo.guru/wiki/First_steps_of_SuperMemo
 * https://www.super-memory.com/
 * 