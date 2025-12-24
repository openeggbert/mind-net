import {Option} from "./Option.js";

export class EnumOption extends Option {
    constructor(enum_instance) {
        super(enum_instance.id, enum_instance.label);
    }
}
