export const Language = Object.freeze({
    // Major world languages
    English: {id: 1, label: "English", native: "English", iso639: "en", emoji: "🇬🇧"},
    Chinese: {id: 2, label: "Chinese", native: "中文", iso639: "zh", emoji: "🇨🇳"},
    Spanish: {id: 3, label: "Spanish", native: "Español", iso639: "es", emoji: "🇪🇸"},
    Hindi: {id: 4, label: "Hindi", native: "हिन्दी", iso639: "hi", emoji: "🇮🇳"},
    Arabic: {id: 5, label: "Arabic", native: "العربية", iso639: "ar", emoji: "🇸🇦"},
    Portuguese: {id: 6, label: "Portuguese", native: "Português", iso639: "pt", emoji: "🇵🇹"},
    Bengali: {id: 7, label: "Bengali", native: "বাংলা", iso639: "bn", emoji: "🇧🇩"},
    Russian: {id: 8, label: "Russian", native: "Русский", iso639: "ru", emoji: "🇷🇺"},
    Japanese: {id: 9, label: "Japanese", native: "日本語", iso639: "ja", emoji: "🇯🇵"},
    German: {id: 10, label: "German", native: "Deutsch", iso639: "de", emoji: "🇩🇪"},

    // European languages
    French: {id: 11, label: "French", native: "Français", iso639: "fr", emoji: "🇫🇷"},
    Italian: {id: 12, label: "Italian", native: "Italiano", iso639: "it", emoji: "🇮🇹"},
    Czech: {id: 13, label: "Czech", native: "Čeština", iso639: "cs", emoji: "🇨🇿"},
    Slovak: {id: 14, label: "Slovak", native: "Slovenčina", iso639: "sk", emoji: "🇸🇰"},
    Polish: {id: 15, label: "Polish", native: "Polski", iso639: "pl", emoji: "🇵🇱"},
    Ukrainian: {id: 16, label: "Ukrainian", native: "Українська", iso639: "uk", emoji: "🇺🇦"},
    Dutch: {id: 17, label: "Dutch", native: "Nederlands", iso639: "nl", emoji: "🇳🇱"},
    Swedish: {id: 18, label: "Swedish", native: "Svenska", iso639: "sv", emoji: "🇸🇪"},
    Norwegian: {id: 19, label: "Norwegian", native: "Norsk", iso639: "no", emoji: "🇳🇴"},
    Danish: {id: 20, label: "Danish", native: "Dansk", iso639: "da", emoji: "🇩🇰"},
    Finnish: {id: 21, label: "Finnish", native: "Suomi", iso639: "fi", emoji: "🇫🇮"},
    Hungarian: {id: 22, label: "Hungarian", native: "Magyar", iso639: "hu", emoji: "🇭🇺"},
    Romanian: {id: 23, label: "Romanian", native: "Română", iso639: "ro", emoji: "🇷🇴"},
    Bulgarian: {id: 24, label: "Bulgarian", native: "Български", iso639: "bg", emoji: "🇧🇬"},
    Serbian: {id: 25, label: "Serbian", native: "Српски", iso639: "sr", emoji: "🇷🇸"},
    Croatian: {id: 26, label: "Croatian", native: "Hrvatski", iso639: "hr", emoji: "🇭🇷"},
    Slovenian: {id: 27, label: "Slovenian", native: "Slovenščina", iso639: "sl", emoji: "🇸🇮"},
    Greek: {id: 28, label: "Greek", native: "Ελληνικά", iso639: "el", emoji: "🇬🇷"},
    Estonian: {id: 29, label: "Estonian", native: "Eesti", iso639: "et", emoji: "🇪🇪"},
    Latvian: {id: 30, label: "Latvian", native: "Latviešu", iso639: "lv", emoji: "🇱🇻"},
    Lithuanian: {id: 31, label: "Lithuanian", native: "Lietuvių", iso639: "lt", emoji: "🇱🇹"},

    // Asian & Middle Eastern languages
    Korean: {id: 40, label: "Korean", native: "한국어", iso639: "ko", emoji: "🇰🇷"},
    Vietnamese: {id: 41, label: "Vietnamese", native: "Tiếng Việt", iso639: "vi", emoji: "🇻🇳"},
    Thai: {id: 42, label: "Thai", native: "ไทย", iso639: "th", emoji: "🇹🇭"},
    Indonesian: {id: 43, label: "Indonesian", native: "Bahasa Indonesia", iso639: "id", emoji: "🇮🇩"},
    Malay: {id: 44, label: "Malay", native: "Bahasa Melayu", iso639: "ms", emoji: "🇲🇾"},
    Tamil: {id: 45, label: "Tamil", native: "தமிழ்", iso639: "ta", emoji: "🇮🇳"},
    Telugu: {id: 46, label: "Telugu", native: "తెలుగు", iso639: "te", emoji: "🇮🇳"},
    Urdu: {id: 47, label: "Urdu", native: "اردو", iso639: "ur", emoji: "🇵🇰"},
    Persian: {id: 48, label: "Persian", native: "فارسی", iso639: "fa", emoji: "🇮🇷"},
    Hebrew: {id: 49, label: "Hebrew", native: "עברית", iso639: "he", emoji: "🇮🇱"},

    // Classical / constructed
    Latin: {id: 60, label: "Latin", native: "Latina", iso639: "la", emoji: "🏛️"},
    Esperanto: {id: 61, label: "Esperanto", native: "Esperanto", iso639: "eo", emoji: "🌍"},
});
export const SUPPORTED_LANGUAGES = [Language.English, Language.Czech]

export function find_language__iso639(iso639) {

    return Object.entries(Language).map(([key, value]) => ({
        key,
        id: value.id,
        label: value.label,
        native: value.native,
        iso639: value.iso639,
        emoji: value.emoji
    }));

}

export class LanguageObject {
    constructor(language) {
        this.id = language.id
        this.label = language.label
        this.native = language.native
        this.iso639 = language.iso639
        this.emoji = language.emoji
    }

    generate_text_for_option() {
        return this.emoji + " " + this.native
    }
}