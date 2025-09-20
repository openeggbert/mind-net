# Chat

 * Use server sent events (SSE)
   * https://github.com/CrowCpp/crow/issues/99

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