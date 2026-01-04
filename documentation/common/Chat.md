# Chat

## New tables

### New table comment_reaction

```
CREATE TABLE comment_reaction (
comment_id INTEGER,
user_id INTEGER NOT NULL,
type TEXT NOT NULL, -- např. 'like', 'heart', 'laugh'
FOREIGN KEY(comment_id) REFERENCES comment(id),
FOREIGN KEY(user_id) REFERENCES user(id)
);
```

### New table discussion_read_status

```
CREATE TABLE discussion_read_status (
user_id INTEGER NOT NULL,
discussion_id INTEGER NOT NULL,
last_read_at DATETIME DEFAULT CURRENT_TIMESTAMP,
PRIMARY KEY(user_id, discussion_id),
FOREIGN KEY(user_id) REFERENCES user(id),
FOREIGN KEY(discussion_id) REFERENCES discussion(id)
);
```

## UI
🎨 What makes Slack like Slack (and what you can replicate)
✅ 1. Purple Theme

    Main color: #4A154B (Slack purple) 
    Accents: lighter purple, white text, blue notifications

✅ 2. Layout

    Left panel: channel list (in your case Discussion)
    Main panel: messages (Comments) with threading
    Top bar: discussion name, actions (search, settings)

✅ 3. Messages

    Bubbles with avatars, name, timestamp
    Markdown support, emoji, reactions
    Threading (reply to specific message)

✅ 4. Interactions

    Realtime feel (even though not WebSocket)
    Hover effects, animations, smooth scroll
    Reply, edit, delete, reactions

💡 UX tip: "Slack-like" is not just color, but feeling

    Message send animation
    Message reactions as icons below
    Threading with visual indentation
    Hover effects on messages
    Smooth transitions between discussions