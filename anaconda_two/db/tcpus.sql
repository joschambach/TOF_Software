-- TCPUs
drop table if exists tcpus;
create table tcpus (
    id                  integer primary key autoincrement,
    device_id           integer not null,
    canbus_id           integer not null,
    tray_id             integer not null,
    tray_sn             char(3) not null,
    thub_id             integer not null,
    serdes              char(2) not null,
    lv_box              integer not null,
    lv_ch               integer not null,
    hv_box              integer not null,
    hv_ch               integer not null,
    installed           boolean  not null default 0
);

-- west
insert into tcpus values (  1, 3, 43,  72,  68,  3, 'C2',  2,  0,  1,  1,  1);
insert into tcpus values (  2, 3, 44,  58,  58,  3, 'C3',  2,  1,  1,  2,  1);
insert into tcpus values (  3, 3, 45,  67,  63,  3, 'D0',  2,  2,  1,  3,  1);
insert into tcpus values (  4, 3, 46,   9,   6,  3, 'D1',  2,  3,  1,  4,  1);
insert into tcpus values (  5, 3, 47,  75,  71,  3, 'D2',  2,  4,  1,  5,  1);
insert into tcpus values (  6, 3, 48,  71,  67,  3, 'D3',  2,  5,  1,  6,  1);
insert into tcpus values (  7, 3, 49,  65,  61,  3, 'E0',  2,  6,  1,  7,  1);
insert into tcpus values (  8, 3, 50,  54,  50,  3, 'E1',  2,  7,  1,  8,  1);
insert into tcpus values (  9, 3, 51,  61,  57,  3, 'E2',  2,  8,  1,  9,  1);
insert into tcpus values ( 10, 3, 52,  64,  60,  3, 'E3',  2,  9,  1, 10,  1);
insert into tcpus values ( 11, 3, 53,  51,  47,  3, 'F0',  2, 10,  1, 11,  1);
insert into tcpus values ( 12, 3, 54,  50,  46,  3, 'F1',  2, 11,  1, 12,  1);
insert into tcpus values ( 13, 3, 55,  -1,  '',  3,  '',  -1,  0, -1,  0,  0);
insert into tcpus values ( 14, 3, 56,  -1,  '',  3,  '',  -1,  0, -1,  0,  0);
insert into tcpus values ( 15, 3, 57,  68,  64,  3,  'G0',  8,  2,  2,  3,  1);
insert into tcpus values ( 16, 3, 58,  76,  72,  3, 'G1',  8,  3,  2,  4,  1);
insert into tcpus values ( 17, 3, 59,  66,  62,  3, 'G2',  8,  4,  2,  5,  1);
insert into tcpus values ( 18, 3, 60,  46,  42,  3, 'G3',  8,  5,  2,  6,  1);
insert into tcpus values ( 19, 3, 61,  18,  14,  3, 'H0',  8,  6,  2,  7,  1);
insert into tcpus values ( 20, 3, 62,  14,  11,  3, 'H1',  8,  7,  2,  8,  1);

insert into tcpus values ( 21, 1, 33,  32,  28,  1, 'A0',  8,  8,  2,  9,  1);
insert into tcpus values ( 22, 1, 34,  10,   7,  1, 'A1',  8,  9,  2, 10,  1);
insert into tcpus values ( 23, 1, 35,  29,  25,  1, 'A2',  8, 10,  2, 11,  1);
insert into tcpus values ( 24, 1, 36,  37,  33,  1, 'A3',  8, 11,  2, 12,  1);
insert into tcpus values ( 25, 1, 37,  23,  19,  1, 'B0',  9,  0,  3,  1,  1);
insert into tcpus values ( 26, 1, 38,  21,  17,  1, 'B1',  9,  1,  3,  2,  1);
insert into tcpus values ( 27, 1, 39,  41,  37,  1, 'B2',  9,  2,  3,  3,  1);
insert into tcpus values ( 28, 1, 40,  25,  21,  1, 'B3',  9,  3,  3,  4,  1);
insert into tcpus values ( 29, 1, 41,  34,  30,  1, 'C0',  9,  4,  3,  5,  1);
insert into tcpus values ( 30, 1, 42,  39,  35,  1, 'C1',  9,  5,  3,  6,  1);
insert into tcpus values ( 31, 1, 43,  11,   8,  1, 'C2',  9,  6,  3,  7,  1);
insert into tcpus values ( 32, 1, 44,  24,  20,  1, 'C3',  9,  7,  3,  8,  1);
insert into tcpus values ( 33, 1, 45,  35,  31,  1, 'D0',  9,  8,  3,  9,  1);
insert into tcpus values ( 34, 1, 46,  31,  27,  1, 'D1',  9,  9,  3, 10,  1);
insert into tcpus values ( 35, 1, 47,  15,  12,  1, 'D2',  9, 10,  3, 11,  1);
insert into tcpus values ( 36, 1, 48,  12,   9,  1, 'D3',  9, 11,  3, 12,  1);
insert into tcpus values ( 37, 1, 49,  13,  10,  1, 'E0',  0,  0,  4,  1,  1);
insert into tcpus values ( 38, 1, 50,  33,  29,  1, 'E1',  0,  1,  4,  2,  1);
insert into tcpus values ( 39, 1, 51,  38,  34,  1, 'E2',  0,  2,  4,  3,  1);
insert into tcpus values ( 40, 1, 52,  16,  13,  1, 'E3',  0,  3,  4,  4,  1);
insert into tcpus values ( 41, 1, 53,  20,  16,  1, 'F0',  0,  4,  4,  5,  1);
insert into tcpus values ( 42, 1, 54,  -1,  '',  1,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 43, 1, 55,  -1,  '',  1,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 44, 1, 56,  40,  36,  1, 'F3',  0,  7,  4,  8,  1);
insert into tcpus values ( 45, 1, 57,  43,  39,  1, 'G0',  0,  8,  4,  9,  1);
insert into tcpus values ( 46, 1, 58,  27,  23,  1, 'G1',  0,  9,  4, 10,  1);
insert into tcpus values ( 47, 1, 59,  36,  32,  1, 'G2',  0, 10,  4, 11,  1);
insert into tcpus values ( 48, 1, 60,  19,  15,  1, 'G3',  0, 11,  4, 12,  1);
insert into tcpus values ( 49, 1, 61,  28,  24,  1, 'H0',  1,  0,  5,  1,  1);
insert into tcpus values ( 50, 1, 62,  22,  18,  1, 'H1',  1,  1,  5,  2,  1);

insert into tcpus values ( 51, 3, 33,  63,  59,  3, 'A0',  1,  2,  5,  3,  1);
insert into tcpus values ( 52, 3, 34,  70,  66,  3, 'A1',  1,  3,  5,  4,  1);
insert into tcpus values ( 53, 3, 35,  74,  70,  3, 'A2',  1,  4,  5,  5,  1);
insert into tcpus values ( 54, 3, 36,  42,  38,  3, 'A3',  1,  5,  5,  6,  1);
insert into tcpus values ( 55, 3, 37,  45,  41,  3, 'B0',  1,  6,  5,  7,  1);
insert into tcpus values ( 56, 3, 38,  49,  45,  3, 'B1',  1,  7,  5,  8,  1);
insert into tcpus values ( 57, 3, 39,  56,  52,  3, 'B2',  1,  8,  5,  9,  1);
insert into tcpus values ( 58, 3, 40,  58,  54,  3, 'B3',  1,  9,  5, 10,  1);
insert into tcpus values ( 59, 3, 41,  57,  53,  3, 'C0',  1, 10,  5, 11,  1);
insert into tcpus values ( 60, 3, 42,  59,  55,  3, 'C1',  1, 11,  5, 12,  1);


-- east
insert into tcpus values ( 61, 4, 58,  88,  84,  4, 'G1',  3,  0,  6,  1,  1);
insert into tcpus values ( 62, 4, 59,  79,  75,  4, 'G2',  3,  1,  6,  2,  1);
insert into tcpus values ( 63, 4, 60,  98,  94,  4, 'G3',  3,  2,  6,  3,  1);
insert into tcpus values ( 64, 4, 61,  95,  91,  4, 'H0',  3,  3,  6,  4,  1);
insert into tcpus values ( 65, 4, 62,  99,  95,  4, 'H1',  3,  4,  6,  5,  1);

insert into tcpus values ( 66, 2, 33,  52,  48,  2, 'A0',  3,  5,  6,  6,  1);
insert into tcpus values ( 67, 2, 34,  69,  65,  2, 'A1',  3,  6,  6,  7,  1);
insert into tcpus values ( 68, 2, 35,  26,  22,  2, 'A2',  3,  7,  6,  8,  1);
insert into tcpus values ( 69, 2, 36,  44,  40,  2, 'A3',  3,  8,  6,  9,  1);
insert into tcpus values ( 70, 2, 37,  53,  49,  2, 'B0',  3,  9,  6, 10,  1);
insert into tcpus values ( 71, 2, 38,   8,   5,  2, 'B1',  3, 10,  6, 11,  1);
insert into tcpus values ( 72, 2, 39,   5,   2,  2, 'B2',  3, 11,  6, 12,  1);
insert into tcpus values ( 73, 2, 40,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 74, 2, 41,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 75, 2, 42,   4,   1,  2, 'C1',  4,  2,  7,  3,  1);
insert into tcpus values ( 76, 2, 43,   7,   4,  2, 'C2',  4,  3,  7,  4,  1);
insert into tcpus values ( 77, 2, 44,   6,   3,  2, 'C3',  4,  4,  7,  5,  1);
insert into tcpus values ( 78, 2, 45,  55,  51,  2, 'D0',  4,  5,  7,  6,  1);
insert into tcpus values ( 79, 2, 46,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 80, 2, 47,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 81, 2, 48,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 82, 2, 49,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 83, 2, 50,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 84, 2, 51,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 85, 2, 52,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 86, 2, 53,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 87, 2, 54,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 88, 2, 55,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 89, 2, 56,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 90, 2, 57,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 91, 2, 58,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 92, 2, 59,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 93, 2, 60,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 94, 2, 61,  -1,  '',  2,   '', -1,  0, -1,  0,  0);
insert into tcpus values ( 95, 2, 62,  -1,  '',  2,   '', -1,  0, -1,  0,  0);

insert into tcpus values ( 96, 4, 33,  -1,  '',  4,    '', -1,  0, -1,  0,  0);
insert into tcpus values ( 97, 4, 34,  81,  77,  4,  'A1',  6,  0,  9,  1,  1);
insert into tcpus values ( 98, 4, 35,  93,  89,  4,  'A2',  6,  1,  9,  2,  1);
insert into tcpus values ( 99, 4, 36,  91,  87,  4,  'A3',  6,  2,  9,  3,  1);
insert into tcpus values (100, 4, 37,  94,  90,  4,  'B0',  6,  3,  9,  4,  1);
insert into tcpus values (101, 4, 38,  97,  93,  4,  'B1',  6,  4,  9,  5,  1);
insert into tcpus values (102, 4, 39,  -1,  '',  4,    '', -1,  0, -1,  0,  0);
insert into tcpus values (103, 4, 40,  -1,  '',  4,    '', -1,  0, -1,  0,  0);
insert into tcpus values (104, 4, 41,  77,  73,  4,  'C0',  6,  7,  9,  8,  1);
insert into tcpus values (105, 4, 42,  80,  76,  4,  'C1',  6,  8,  9,  9,  1);
insert into tcpus values (106, 4, 43,  87,  83,  4,  'C2',  6,  9,  9, 10,  1);
insert into tcpus values (107, 4, 44,  73,  69,  4,  'C3',  6, 10,  9, 11,  1);
insert into tcpus values (108, 4, 45,  84,  80,  4,  'D0',  6, 11,  9, 12,  1);

insert into tcpus values (109, 4, 46,  86,  82,  4,  'D1',  7,  0, 10,  1,  1);
insert into tcpus values (110, 4, 47,  78,  74,  4,  'D2',  7,  1, 10,  2,  1);
insert into tcpus values (111, 4, 48,   3, 'C',  4,  'D3',  7,  2, 10,  3,  1);
insert into tcpus values (112, 4, 49,  83,  79,  4,  'E0',  7,  3, 10,  4,  1);
insert into tcpus values (113, 4, 50,  47,  43,  4,  'E1',  7,  4, 10,  5,  1);
insert into tcpus values (114, 4, 51,  90,  86,  4,  'E2',  7,  5, 10,  6,  1);
insert into tcpus values (115, 4, 52,  89,  85,  4,  'E3',  7,  6, 10,  7,  1);
insert into tcpus values (116, 4, 53,   2, 'B',  4,  'F0',  7,  7, 10,  8,  1);
insert into tcpus values (117, 4, 54,  85,  81,  4,  'F1',  7,  8, 10,  9,  1);
insert into tcpus values (118, 4, 55,  96,  92,  4,  'F2',  7,  9, 10, 10,  1);
insert into tcpus values (119, 4, 56, 100,  96,  4,  'F3',  7, 10, 10, 11,  1);
insert into tcpus values (120, 4, 57,  84,  78,  4,  'G0',  7, 11, 10, 12,  1);

-- upVPD WEST
insert into tcpus values (121, 5, 32,   0,  'W', 1,  'H2', 10,  9,  0,  0,  1);
-- upVPD EAST
insert into tcpus values (122, 6, 32,   0,  'E', 2,  'H0', 10,  8,  0,  0,  1);
