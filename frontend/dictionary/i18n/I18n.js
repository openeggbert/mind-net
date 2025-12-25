import {defined} from "../../common.js";

export class I18n {
    #fallbackLang
    #fallbackMap

    constructor(defaultLang = "en", fallbackLang = "en") {
        this.lang = defaultLang;
        this.#fallbackLang = fallbackLang

        this.map = new Map();
        this.#fallbackMap = new Map()
    }

    async load(lang = null) {
        if (lang) {
            this.lang = lang;
        }

        this.map.clear();
        const json = await fetch(
            `/web/dictionary/i18n/${this.lang}.json`
        ).then(r => r.json());
        for (const [k, v] of Object.entries(json)) {
            this.map.set(k, String(v));
            //console.log(k + "=" + String(v))
        }

        {
            this.#fallbackMap.clear();
            const json = await fetch(
                `/web/dictionary/i18n/${this.#fallbackLang}.json`
            ).then(r => r.json());
            for (const [k, v] of Object.entries(json)) {
                this.#fallbackMap.set(k, String(v));
            }
        }

        return this;
    }


    t(key, params = {}) {
        let text = (
            this.map.get(key) ??
            this.#fallbackMap?.get(key) ??
            `⟨${key}⟩`
        );

        for (const [name, value] of Object.entries(params)) {
            console.log("Going to replace " + name  + " by " + value)
            text = text.replaceAll(`{${name}}`, String(value));
        }

        return text
    }

    has(key) {
        return this.map.has(key);
    }

    applyTranslations() {
        document.querySelectorAll("[data-i18n]").forEach(el => {
            let params = {}
            if(el.dataset.i18nParams) {

                el.dataset.i18nParams.split("::::").forEach(e=>{
                    let param = e.split("::")
                    let value = param[1]?? ""
                    if(value.length > 0) {
                        value = this.t(value)
                    }
                    params[param[0]] = value
                })

            }
            el.textContent = this.t(el.dataset.i18n, params);

        });

        document.querySelectorAll("[data-i18n-placeholder]").forEach(el => {
            el.placeholder = this.t(el.dataset.i18nPlaceholder);
        });

        document
            .querySelectorAll("[data-i18n-title]")
            .forEach(el =>
                el.title = this.t(el.dataset.i18nTitle)
            );



        return this;
    }

    async setLanguage(lang) {
        await this.load(lang);
        this.applyTranslations();
        localStorage.setItem("lang", lang);
    }

    getLanguage() {
        return localStorage.getItem("lang") ?? "en";
    }

}

