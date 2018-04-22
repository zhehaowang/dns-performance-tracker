CREATE DATABASE IF NOT EXISTS dpt;

USE dpt;

CREATE TABLE IF NOT EXISTS domain_stats (
    id              INT  NOT NULL AUTO_INCREMENT,
    domain          VARCHAR(255) NOT NULL,
    variance        DOUBLE DEFAULT 0.0,
    number_success  BIGINT UNSIGNED DEFAULT 0,
    number_fail     BIGINT UNSIGNED DEFAULT 0,
    total_ns        BIGINT UNSIGNED DEFAULT 0,
    first_time      TIME,
    last_time       TIME,
    PRIMARY KEY(id),
    INDEX(domain)
) ENGINE = InnoDB;

CREATE TABLE IF NOT EXISTS query_record (
    id              INT  NOT NULL AUTO_INCREMENT,
    domain          TEXT,
    status          TEXT,
    total_ns        BIGINT UNSIGNED DEFAULT 0,
    timestamp       TIME,
    PRIMARY KEY(id)
) ENGINE = InnoDB;