export interface UdongConfig {
    analog_switches: Array<AnalogSwitchConfig>
}

export interface AnalogSwitchConfig {
    id: number,
    trigger: RapidTrigger | StaticTrigger,
}

export interface RapidTrigger {
    type: string;
    act: number;
    rel: number;
    f_act: number;
    f_rel: number;
}

export interface StaticTrigger {
    type: string;
    act: number;
    rel: number;
}