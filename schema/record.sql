CREATE DATABASE IF NOT EXISTS dpt;

USE dpt;

CREATE TABLE IF NOT EXISTS domain_stats (
    domain          VARCHAR(255) NOT NULL,
    variance        DOUBLE DEFAULT 0.0,
    number_success  BIGINT UNSIGNED DEFAULT 0,
    number_fail     BIGINT UNSIGNED DEFAULT 0,
    total_duration  BIGINT UNSIGNED DEFAULT 0,
    first_time      TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_time       TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY(domain)
) ENGINE = InnoDB;

CREATE TABLE IF NOT EXISTS query_record (
    domain          TEXT,
    status          TEXT,
    duration        BIGINT UNSIGNED DEFAULT 0,
    timestamp       TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    id              INT  NOT NULL AUTO_INCREMENT,
    PRIMARY KEY(id)
) ENGINE = InnoDB;