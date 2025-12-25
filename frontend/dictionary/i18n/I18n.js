export class I18n {
    constructor(defaultLang = "en") {
        this.lang = defaultLang;
        this.map = new Map();
        this.fallback = null;
    }

    async load(lang) {
        this.lang = lang;
        this.map.clear();

        const json = await fetch(`/dictionary/i18n/${lang}.json`)
            .then(r => r.json());

        for (const [k, v] of Object.entries(json)) {
            this.map.set(k, String(v));
        }
    }

    t(key) {
        return (
            this.map.get(key) ??
            this.fallback?.get(key) ??
            `⟨${key}⟩`
        );
    }

    has(key) {
        return this.map.has(key);
    }
}
