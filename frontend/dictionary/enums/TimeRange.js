export const TimeRange = Object.freeze({
    Any: {id: 0, label: "Any"},

    LastHour: {id: 1, label: "Last hour"},
    Last3Hours: {id: 2, label: "Last 3 hours"},
    Last24Hours: {id: 3, label: "Last 24 hours"},
    LastWeek: {id: 4, label: "Last week"},
    LastMonth: {id: 5, label: "Last month"},
    LastYear: {id: 6, label: "Last year"},
    Last10Years: {id: 7, label: "Last 10 years"},

    NotLastHour: {id: 21, label: "Not last hour"},
    NotLast3Hours: {id: 22, label: "Not last 3 hours"},
    NotLast24Hours: {id: 23, label: "Not last 24 hours"},
    NotLastWeek: {id: 24, label: "Not last week"},
    NotLastMonth: {id: 25, label: "Not last month"},
    NotLastYear: {id: 26, label: "Not last year"},
    NotLast10Years: {id: 27, label: "Not last 10 years"},

    Never: {id: 100, label: "Never"},
});