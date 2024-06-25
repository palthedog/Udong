import { Provider } from '@angular/core';
import log, { getLogger } from 'loglevel';
import { environment } from '../environments/environment';

export class Logger {
    impl_: log.Logger;

    constructor(impl: log.Logger) {
        this.impl_ = impl;
    }

    trace(...msg: any[]) {
        this.impl_.trace(...msg);
    }

    debug(...msg: any[]) {
        this.impl_.debug(...msg);
    }

    info(...msg: any[]) {
        this.impl_.info(...msg);
    }

    warn(...msg: any[]) {
        this.impl_.warn(...msg);
    }

    error(...msg: any[]) {
        this.impl_.error(...msg);
    }
};

function loggerFactory() {
    let logger = getLogger('WebConf');
    // Hrm... How can I make Typescript happy?
    if (environment.logLevel == 'trace' ||
        environment.logLevel == 'debug' ||
        environment.logLevel == 'info' ||
        environment.logLevel == 'warn' ||
        environment.logLevel == 'error' ||
        environment.logLevel == 'silent') {
        logger.setLevel(environment.logLevel);
    } else {
        console.warn('Unknown log level is specified in the environment config:', environment.logLevel);
    }
    return new Logger(logger);
}

export function provideLogger(): Provider {
    return { provide: Logger, useFactory: loggerFactory };
};
