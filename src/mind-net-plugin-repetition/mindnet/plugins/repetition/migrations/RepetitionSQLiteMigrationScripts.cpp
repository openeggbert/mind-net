//
// Created by robertvokac on 9/17/25.
//

#include "mindnet/plugins/repetition/migrations/RepetitionSQLiteMigrationScripts.h"

namespace mindnet::plugins::repetition::migrations
{
    RepetitionSQLiteMigrationScripts::RepetitionSQLiteMigrationScripts() : MigrationScripts(
        mindnet::essential::DatabaseType::SQLite)
    {
    }

    void RepetitionSQLiteMigrationScripts::define_migrations()
    {
        add_migration("V1__create_r_global_setting.sql", R"(
    	CREATE TABLE r_global_setting (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT (unixepoch(CURRENT_TIMESTAMP) * 1000),
	updated_at DATETIME DEFAULT (unixepoch(CURRENT_TIMESTAMP) * 1000),

	key TEXT NOT NULL,
	value TEXT NOT NULL,

	UNIQUE (key)
);

)");

        //   	INSERT OR IGNORE INTO r_global_setting(key, value) VALUES
        // ('b', 0.6),
        // ('R_target', 0.9),
        // ('R_opt', 0.9),
        // ('alpha', 0.3),
        // ('beta', 0.6),
        // ('gamma', 0.2),
        // ('delta', 0.4),
        // ('k_over', 0.15),
        // ('S_min', 0.5),
        // ('short_retry', 0.5); -- 12 hours


        add_migration("V2__create_r_user_setting.sql", R"(
CREATE TABLE r_user_setting (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    key TEXT NOT NULL,
    value TEXT NOT NULL,

    UNIQUE (user_id, key),

    FOREIGN KEY (user_id) REFERENCES user(id)
);
)");

        add_migration("V3__create_r_session.sql", R"(
CREATE TABLE r_session (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    map_id INTEGER NOT NULL,
    cloned_from_session_id INTEGER,

    algorithm INTEGER NOT NULL,
    schedule INTEGER NOT NULL,
    scope INTEGER NOT NULL,

    description TEXT,

    filter_under_note INTEGER,
    filter_date_from DATETIME,
    filter_date_to DATETIME,
    filter_tag INTEGER,
    filter_collection INTEGER,

    selected_items TEXT NOT NULL DEFAULT '{}',
    pinned BOOL DEFAULT 0,

    FOREIGN KEY (user_id) REFERENCES user(id),
    FOREIGN KEY (map_id) REFERENCES map(id),
    FOREIGN KEY (cloned_from_session_id) REFERENCES r_session(id),
    FOREIGN KEY (filter_under_note) REFERENCES note(id),
    FOREIGN KEY (filter_tag) REFERENCES tag_type(id),
    FOREIGN KEY (filter_collection) REFERENCES collection(id)
);

)");

        add_migration("V4__create_r_review.sql", R"(
CREATE TABLE r_review (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    map_id INTEGER NOT NULL,
    r_session_id INTEGER,

    algorithm INTEGER NOT NULL,

    note_id INTEGER,

    review_date DATETIME,
    grade INTEGER CHECK (grade BETWEEN 0 AND 5),
    response_data TEXT, -- for example. JSON: {"selected": [1, 3]}
    notes TEXT,

	started_at DATETIME,   -- when started answering
	ended_at DATETIME,	 -- when finished
	latency_ms INTEGER,	-- response time in ms

	-- user behavior
	answer_change_count INTEGER DEFAULT 0,

    details_json TEXT NOT NULL DEFAULT '{}',
	-- optional JSON with algorithm-specific details
	-- e.g. SM-18: {"R_before":0.72,"S_before":2.1,"S_after":2.4,"next_interval":4.5}

    FOREIGN KEY (user_id) REFERENCES user(id),
    FOREIGN KEY (r_session_id) REFERENCES r_session(id),
    FOREIGN KEY (note_id) REFERENCES note(id)

);

)");
        add_migration("V5__create_r0_state.sql", R"(
CREATE TABLE r0_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

	user_id INTEGER NOT NULL,
	note_id INTEGER,

	repetitions INTEGER DEFAULT 0,	  -- number of completed repetitions 
	interval INTEGER DEFAULT 1,		 -- current interval in days (from fixed sequence)
	next_review DATETIME,			   -- when next repetition should occur
	last_review DATETIME,			   -- when last repetition occurred
	last_quality INTEGER DEFAULT 0,	 -- last grade

	UNIQUE (user_id, note_id),

	FOREIGN KEY (note_id) REFERENCES note(id),
	FOREIGN KEY (user_id) REFERENCES user(id)
);

)");

        add_migration("V6__create_r2_state.sql", R"(
CREATE TABLE r2_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

    user_id INTEGER NOT NULL,
    note_id INTEGER,

    repetitions INTEGER DEFAULT 0,
    interval INTEGER DEFAULT 1,
    ef_times_100 INTEGER DEFAULT 250 CHECK (ef_times_100 >= 100 and ef_times_100 <= 500),

    next_review DATETIME,
    last_review DATETIME,
    last_quality INTEGER DEFAULT 0,

    UNIQUE (user_id, note_id),

    FOREIGN KEY (note_id) REFERENCES note(id),
    FOREIGN KEY (user_id) REFERENCES user(id)
);
)");

        add_migration("V7__create_r4_state.sql", R"(
CREATE TABLE r4_state (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

	user_id INTEGER NOT NULL,
	note_id INTEGER,

	repetitions INTEGER DEFAULT 0,		  -- which repetition 
	interval INTEGER DEFAULT 1,			 -- current interval (days)
	ef_times_100 INTEGER DEFAULT 250 
		CHECK (ef_times_100 >= 100 and ef_times_100 <= 500),
											 -- E-Factor * 100
	correction_factor_times_100 INTEGER DEFAULT 100, 	 -- new: coefficient for interval correction
											-- (SM-4 adds adaptive adjustments)

	next_review DATETIME,				   -- when next repetition should occur
	last_review DATETIME,				   -- when last repetition occurred
	last_quality INTEGER DEFAULT 0,		 -- last grade (0-5)

	UNIQUE (user_id, note_id),

	FOREIGN KEY (note_id) REFERENCES note(id),
	FOREIGN KEY (user_id) REFERENCES user(id)
);
)");


        add_migration("V8__create_r18_state.sql", R"(
CREATE TABLE r18_state (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    user_id INTEGER NOT NULL,
    note_id INTEGER,

    stability_times_100 INTEGER DEFAULT 100,    -- S
	last_interval_times_100 INTEGER DEFAULT 0,  -- last interval (days)
    repetitions INTEGER DEFAULT 0,
    lapses INTEGER DEFAULT 0,
    next_review DATETIME,
    last_review DATETIME,
    last_quality INTEGER DEFAULT 0,

    UNIQUE (user_id, note_id),

    FOREIGN KEY (note_id) REFERENCES note(id),
    FOREIGN KEY (user_id) REFERENCES user(id)
);
)");

        add_migration("V9__create_r18_perf_agg.sql", R"(
CREATE TABLE r18_perf_agg (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,

    user_id INTEGER NOT NULL,
    bin_log_t_times_100 INTEGER NOT NULL,  -- log(t) × 100
    total INTEGER NOT NULL,
    correct INTEGER NOT NULL,

    UNIQUE (user_id, bin_log_t_times_100),

    FOREIGN KEY (user_id) REFERENCES user(id)
);

    	)");

        add_migration("V10__create_r18_prediction_log.sql", R"(
CREATE TABLE r18_prediction_log (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    updated_at DATETIME,
	user_id INTEGER NOT NULL,
	note_id INTEGER,
	predicted_R_times_100 INTEGER NOT NULL,
	actual_grade INTEGER NOT NULL,
	was_correct BOOLEAN NOT NULL,
	FOREIGN KEY (user_id) REFERENCES user(id),
	FOREIGN KEY (note_id) REFERENCES note(id)
);
)");


        add_migration("V11__create_repetition_indexes.sql", R"(

-- Index for fast selection of "due items" (SM-18 and other algorithms)
CREATE INDEX IF NOT EXISTS idx_r18_state_user_due
    ON r18_state (user_id, next_review);

CREATE INDEX IF NOT EXISTS idx_r4_state_user_due
    ON r4_state (user_id, next_review);

CREATE INDEX IF NOT EXISTS idx_r2_state_user_due
    ON r2_state (user_id, next_review);

CREATE INDEX IF NOT EXISTS idx_r0_state_user_due
    ON r0_state (user_id, next_review);

-- Indexes for fast lookups by note_id
-- in review log and state tables

-- r_review
CREATE INDEX IF NOT EXISTS idx_r_review_note
    ON r_review (note_id);

-- r0_state
CREATE INDEX IF NOT EXISTS idx_r0_state_note
    ON r0_state (note_id);

-- r2_state
CREATE INDEX IF NOT EXISTS idx_r2_state_note
    ON r2_state (note_id);

-- r4_state
CREATE INDEX IF NOT EXISTS idx_r4_state_note
    ON r4_state (note_id);

-- r18_state
CREATE INDEX IF NOT EXISTS idx_r18_state_note
    ON r18_state (note_id);

)");



    	add_migration("V12__alter_r_session_add_map_collection_id.sql", R"(
-- FK_OFF

-- 1) Prepare new table with correct schema
CREATE TABLE r_session_new (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME,
	updated_at DATETIME,

	user_id INTEGER NOT NULL,
	map_id INTEGER,				  -- no longer NOT NULL
	map_collection_id INTEGER,	   -- new field
	cloned_from_session_id INTEGER,

	algorithm INTEGER NOT NULL,
	schedule INTEGER NOT NULL,
	scope INTEGER NOT NULL,

	description TEXT,

	filter_under_note INTEGER,
	filter_date_from DATETIME,
	filter_date_to DATETIME,
	filter_tag INTEGER,
	filter_collection INTEGER,

	selected_items TEXT NOT NULL DEFAULT '{}',
	pinned BOOL DEFAULT 0,

	FOREIGN KEY (user_id) REFERENCES user(id),
	FOREIGN KEY (map_id) REFERENCES map(id),
	FOREIGN KEY (map_collection_id) REFERENCES map_collection(id),
	FOREIGN KEY (cloned_from_session_id) REFERENCES r_session(id), -- self-FK will match after renaming
	FOREIGN KEY (filter_under_note) REFERENCES note(id),
	FOREIGN KEY (filter_tag) REFERENCES tag_type(id),
	FOREIGN KEY (filter_collection) REFERENCES collection(id)
);

-- 2) Transfer data (preserving IDs!)
INSERT INTO r_session_new (
	id, created_at, updated_at,
	user_id, map_id, map_collection_id, cloned_from_session_id,
	algorithm, schedule, scope,
	description,
	filter_under_note, filter_date_from, filter_date_to,
	filter_tag, filter_collection,
	selected_items, pinned
)
SELECT
	id, created_at, updated_at,
	user_id, map_id, NULL AS map_collection_id, cloned_from_session_id,
	algorithm, schedule, scope,
	description,
	filter_under_note, filter_date_from, filter_date_to,
	filter_tag, filter_collection,
	selected_items, pinned
FROM r_session;

-- 3) Drop old table and rename new one
DROP TABLE r_session;
ALTER TABLE r_session_new RENAME TO r_session;

)");

    	add_migration("V13__add_indexes_to_r_session.sql", R"(
-- Extra indexes for fast filtering in UI and queries

-- Already created in V12, but repeat safely in case some DBs missed it
CREATE INDEX IF NOT EXISTS idx_r_session_user_id
    ON r_session(user_id);

-- Lookup by map
CREATE INDEX IF NOT EXISTS idx_r_session_map_id
    ON r_session(map_id);

-- Lookup by collection
CREATE INDEX IF NOT EXISTS idx_r_session_map_collection_id
    ON r_session(map_collection_id);

-- Combined filters used in API / UI
CREATE INDEX IF NOT EXISTS idx_r_session_user_map
    ON r_session(user_id, map_id);

CREATE INDEX IF NOT EXISTS idx_r_session_user_collection
    ON r_session(user_id, map_collection_id);

-- Find pinned sessions for user quickly
CREATE INDEX IF NOT EXISTS idx_r_session_user_pinned
    ON r_session(user_id, pinned);

-- If you ever query clone history
CREATE INDEX IF NOT EXISTS idx_r_session_cloned_from
    ON r_session(cloned_from_session_id);
)");

    	add_migration("V14__alter_r0_state_table_add_column_last_seen_semantic_version.sql", R"(
ALTER TABLE r0_state ADD last_seen_semantic_version INTEGER DEFAULT 1;
)");
    	add_migration("V15__alter_r2_state_table_add_column_last_seen_semantic_version.sql", R"(
ALTER TABLE r2_state ADD last_seen_semantic_version INTEGER DEFAULT 1;
)");
    	add_migration("V16__alter_r4_state_table_add_column_last_seen_semantic_version.sql", R"(
ALTER TABLE r4_state ADD last_seen_semantic_version INTEGER DEFAULT 1;
)");
    	add_migration("V17__alter_r18_state_table_add_column_last_seen_semantic_version.sql", R"(
ALTER TABLE r18_state ADD last_seen_semantic_version INTEGER DEFAULT 1;
)");

    	add_migration("V18__alter_r0_state_table_add_column_content_modified_since_last_review.sql", R"(
ALTER TABLE r0_state ADD content_modified_since_last_review INTEGER DEFAULT 0;
)");
    	add_migration("V19__alter_r2_state_table_add_column_content_modified_since_last_review.sql", R"(
ALTER TABLE r2_state ADD content_modified_since_last_review INTEGER DEFAULT 0;
)");
    	add_migration("V20__alter_r4_state_table_add_column_content_modified_since_last_review.sql", R"(
ALTER TABLE r4_state ADD content_modified_since_last_review INTEGER DEFAULT 0;
)");
    	add_migration("V21__alter_r18_state_table_add_column_content_modified_since_last_review.sql", R"(
ALTER TABLE r18_state ADD content_modified_since_last_review INTEGER DEFAULT 0;
)");




    }
}


/*
CREATE TABLE r_session_metric (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   r_session_id INTEGER NOT NULL,
   user_id INTEGER NOT NULL,
   total_items INTEGER NOT NULL,
   correct_items INTEGER NOT NULL,
   avg_latency_ms INTEGER,
   avg_grade REAL,
   FOREIGN KEY (r_session_id) REFERENCES r_session(id)
);

INSERT INTO r_session_metric (r_session_id, user_id, total_items, correct_items, avg_latency_ms, avg_grade)
SELECT
  r_session_id,
  user_id,
  COUNT(*) AS total_items,
  SUM(CASE WHEN grade >= 3 THEN 1 ELSE 0 END) AS correct_items,
  AVG(latency_ms) AS avg_latency_ms,
  AVG(grade) AS avg_grade
FROM r_review
WHERE r_session_id = ?
GROUP BY r_session_id, user_id;
















Here’s your English version formatted as a clear `todo.md`:

````markdown
# TODO – SM-18 Prediction Log

```sql
CREATE TABLE r18_prediction_log (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_at DATETIME,
    user_id INTEGER NOT NULL,
    note_id INTEGER,
    question_id INTEGER,
    predicted_R REAL NOT NULL,
    actual_grade INTEGER NOT NULL,
    was_correct BOOLEAN NOT NULL
);
````

---

## 🧠 Purpose

The **`r18_prediction_log`** table is designed to log the *predicted retrievability* versus the *actual user response*.
It’s a key diagnostic tool for **debugging, tuning, and validating** the SM-18 algorithm.

---

## 🕐 When to Populate It

On each review (if SM-18 is the active algorithm):

1. **Compute retrievability before the review:**

   ```cpp
   double R_before = retrievability(elapsed, S, b);
   ```

2. **Capture the actual user response:**

   * `actual_grade` = rating `0–5`
   * `was_correct` = `grade >= 3`

3. **Insert a new record:**

   ```sql
   INSERT INTO r18_prediction_log (
     created_at, user_id, note_id, question_id,
     predicted_R, actual_grade, was_correct
   ) VALUES (?, ?, ?, ?, ?, ?, ?);
   ```

---

## 📊 What This Enables

### 1. Measure Prediction Accuracy

Compare `predicted_R` vs `was_correct` to see where the model over- or under-estimates recall.

### 2. Tune Model Parameters (e.g. b, α, γ)

If `predicted_R` is high but users often fail, the model is **too optimistic** — parameters can be adjusted accordingly.

### 3. Evaluate New SM-18 Variants

Compare how different formulations of `R(t|S)` perform in predicting user recall.

### 4. Personalization

Track accuracy per user and build **personal learning profiles** or **user-specific parameter sets**.

---

## 📋 Example Analysis Query

```sql
SELECT
  ROUND(predicted_R, 2) AS R_bin,
  COUNT(*) AS total,
  SUM(CASE WHEN was_correct THEN 1 ELSE 0 END) AS correct,
  ROUND(AVG(actual_grade), 2) AS avg_grade
FROM r18_prediction_log
WHERE user_id = ?
GROUP BY R_bin
ORDER BY R_bin;
```

✅ This shows how predicted retrievability corresponds to actual success rates across different recall probabilities.

---

## 🧩 Summary

The `r18_prediction_log` table serves as:

* A **passive log** for model performance analysis
* A **foundation for adaptive parameter tuning**
* A **research and personalization tool** for improving SM-18

---

👉 *Next step:* optionally implement a C++ function that automatically inserts into this table on every review.

```
```






add_migration("V11__create_r18_adaptive_parameters.sql", R"(
CREATE TABLE r18_adaptive_parameters (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at DATETIME DEFAULT (unixepoch(CURRENT_TIMESTAMP) * 1000),
	updated_at DATETIME DEFAULT (unixepoch(CURRENT_TIMESTAMP) * 1000),
	
	user_id INTEGER NOT NULL,
	
	-- Learning parameters (stored as integers multiplied by 100)
	alpha_times_100 INTEGER DEFAULT 30,	   -- learning rate parameter
	gamma_times_100 INTEGER DEFAULT 20,	   -- forgetting rate parameter
	beta_times_100 INTEGER DEFAULT 60,		-- difficulty scaling parameter
	
	-- Performance metrics
	total_reviews INTEGER DEFAULT 0,		  -- total number of reviews
	successful_reviews INTEGER DEFAULT 0,	  -- number of successful reviews
	average_response_time_ms INTEGER DEFAULT 0,-- average response time
	
	-- Adaptation settings
	last_adaptation_time DATETIME,			-- when parameters were last updated
	reviews_since_adaptation INTEGER DEFAULT 0,-- reviews since last parameter update
	minimum_reviews_for_adaptation INTEGER DEFAULT 50, -- minimum reviews needed before adapting
	
	UNIQUE (user_id),
	FOREIGN KEY (user_id) REFERENCES user(id)
);

-- Index for quick parameter lookups by user
CREATE INDEX idx_r18_adaptive_parameters_user 
ON r18_adaptive_parameters(user_id);

-- Initialize parameters for existing users
INSERT OR IGNORE INTO r18_adaptive_parameters (user_id)
SELECT id FROM user;
)");

*/
