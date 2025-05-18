-- Run this in PostgreSQL to initialize the schema

CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(100) UNIQUE NOT NULL,
    password VARCHAR(100) NOT NULL
);

CREATE TABLE locations (
    id SERIAL PRIMARY KEY,
    address TEXT NOT NULL
);

CREATE TABLE delivery_stats (
    id SERIAL PRIMARY KEY,
    delivery_time INTEGER,
    success BOOLEAN
);
