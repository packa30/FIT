-----------------------------------------------------------------------
--                  Odstranenie existujucich tabuliek
-----------------------------------------------------------------------
BEGIN 
    EXECUTE immediate 'DROP TABLE Zamestnanec CASCADE CONSTRAINTS';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP TABLE Klient CASCADE CONSTRAINTS';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP TABLE Platobna_karta CASCADE CONSTRAINTS';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP TABLE Ucet CASCADE CONSTRAINTS';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP TABLE Pobocka CASCADE CONSTRAINTS';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP TABLE Prikaz CASCADE CONSTRAINTS';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP TABLE Sluzba CASCADE CONSTRAINTS';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP TABLE Disponent CASCADE CONSTRAINTS';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP SEQUENCE dept_prikaz';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP SEQUENCE dept_sluzba';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN 
    EXECUTE immediate 'DROP SEQUENCE dept_disp';
    EXCEPTION WHEN others THEN NULL;
END;
/
BEGIN
  EXECUTE immediate 'DROP PROCEDURE rod_cislo';
  EXCEPTION WHEN others THEN NULL;
END;
/



-----------------------------------------------------------------------
--      Vytvorenie tabuliek
-----------------------------------------------------------------------
CREATE TABLE Zamestnanec (
    ID_Zamestnanca CHAR(11),
    --
    meno VARCHAR(20) NOT NULL,
    priezvysko VARCHAR(20) NOT NULL,
    pozicia VARCHAR(20) NOT NULL,   
    tel_cislo VARCHAR(20),
    adresa VARCHAR(20),
    --
    ID_pobocky NUMBER NOT NULL,
    --
    CONSTRAINT PK_zamestnanec PRIMARY KEY (ID_Zamestnanca),
    CONSTRAINT zamestnanec_enum CHECK (pozicia IN ('klientsky pracovnik', 'manager poboËky',
            'manager banky'))
);

CREATE TABLE Pobocka (
    ID_pobocky NUMBER NOT NULL,
    
    --
    nazov VARCHAR(30) NOT NULL,
    Adresa VARCHAR(20) NOT NULL,
    --
    CONSTRAINT PK_ID_pobocky PRIMARY KEY (ID_pobocky)
);

CREATE TABLE Klient (
    ID_klienta CHAR(11),
    --
    meno VARCHAR(20) NOT NULL,
    priezvysko VARCHAR(20) NOT NULL,  
    tel_cislo VARCHAR(20),
    adresa VARCHAR(20),
    --
    CONSTRAINT PK_ID_klienta PRIMARY KEY (ID_klienta)
 
);

CREATE TABLE Ucet (
    cislo_uctu NUMBER(9,0) NOT NULL,
    --
    typ_uctu VARCHAR(20) NOT NULL,
    zostatok NUMERIC(6,2),
    datum_zalozenia DATE NOT NULL,
    --
    ID_Zamestnanca CHAR(11) NOT NULL,
    ID_klienta CHAR(11),
    --
    CONSTRAINT PK_cislo_uctu PRIMARY KEY (cislo_uctu),
    CONSTRAINT ucet_enum CHECK (typ_uctu IN ('osobny', 'studentsky','firemny')),
    CONSTRAINT Ucet CHECK (cislo_uctu > 99999999)
);

CREATE TABLE Platobna_karta (
   cislo_karty NUMBER(16,0) NOT NULL,
    --
    typ_karty VARCHAR(20),
    --
    cislo_uctu NUMBER(9,0) NOT NULL,
    ID_klienta CHAR(11),
    --
    CONSTRAINT PK_Platobna_karta PRIMARY KEY (cislo_karty),
    CONSTRAINT Platobna_karta CHECK (cislo_karty > 999999999999999),
    CONSTRAINT Platobna_karta_enum CHECK (typ_karty IN ('debetna', 'kreditna','predplatena'))
);

CREATE SEQUENCE dept_prikaz START WITH 1 INCREMENT BY 1;
CREATE TABLE Prikaz (
    pk INTEGER NOT NULL ,
    --
    typ_prikazu VARCHAR(20) NOT NULL,
    datum_vykonania DATE NOT NULL,
    ciastka NUMBER(6,0),
    cudzi_ucet NUMBER(16,0),
    --
    cislo_uctu NUMBER(9,0) NOT NULL,
    ID_klienta CHAR(11) NOT NULL,
    ID_Zamestnanca CHAR(11) NOT NULL,
    --
    CONSTRAINT PK_Prikaz PRIMARY KEY (pk)
   
);

CREATE SEQUENCE dept_sluzba START WITH 1 INCREMENT BY 1;
CREATE TABLE Sluzba (
    ID_sluzby INTEGER NOT NULL,
    --
    nazov VARCHAR(20) NOT NULL,
    --
    cislo_uctu NUMBER(9,0) NOT NULL,
    CONSTRAINT PK_ID_sluzby PRIMARY KEY (ID_sluzby)

);

CREATE SEQUENCE dept_disp START WITH 1 INCREMENT BY 1;
CREATE TABLE Disponent (
    ID_zaznam INTEGER NOT NULL,
    --
    ID_klienta CHAR(11) NOT NULL,
    cislo_uctu NUMBER(9,0) NOT NULL,
    --
    CONSTRAINT PK_ID_zaznam PRIMARY KEY (ID_zaznam)

);

-----------------------------------------------------------------------
--                  Uprava tabuliek
-----------------------------------------------------------------------
ALTER TABLE Zamestnanec ADD CONSTRAINT FK_zamestnanec_pobocka
  FOREIGN KEY (ID_pobocky) REFERENCES Pobocka;

ALTER TABLE Platobna_karta ADD CONSTRAINT FK_ucet_karta
  FOREIGN KEY (cislo_uctu) REFERENCES Ucet ON DELETE CASCADE;
ALTER TABLE Platobna_karta ADD CONSTRAINT FK_klient_karta
  FOREIGN KEY (ID_klienta) REFERENCES Klient ON DELETE CASCADE;
  
ALTER TABLE Sluzba ADD CONSTRAINT FK_ucet_sluzba
  FOREIGN KEY (cislo_uctu) REFERENCES Ucet ON DELETE CASCADE;
  
ALTER TABLE Prikaz ADD CONSTRAINT FK_ucet_prikaz
  FOREIGN KEY (cislo_uctu) REFERENCES Ucet ON DELETE CASCADE;
ALTER TABLE Prikaz ADD CONSTRAINT FK_klient_prikaz
  FOREIGN KEY (ID_klienta) REFERENCES Klient;
ALTER TABLE Prikaz ADD CONSTRAINT FK_zamestnanec_prikaz
  FOREIGN KEY (ID_Zamestnanca) REFERENCES Zamestnanec;
  
ALTER TABLE Ucet ADD CONSTRAINT FK_zamestnanec_ucet
  FOREIGN KEY (ID_Zamestnanca) REFERENCES Zamestnanec;
ALTER TABLE Ucet ADD CONSTRAINT FK_klient_ucet
  FOREIGN KEY (ID_Klienta) REFERENCES Klient ON DELETE CASCADE;

ALTER TABLE Disponent ADD CONSTRAINT FK_disponuje
  FOREIGN KEY (ID_Klienta) REFERENCES Klient ON DELETE CASCADE;
ALTER TABLE Disponent ADD CONSTRAINT FK_disponuje_k
  FOREIGN KEY (cislo_uctu) REFERENCES Ucet ON DELETE CASCADE;


-----------------------------------------------------------------------
--                   INSERTY
-----------------------------------------------------------------------

INSERT INTO Pobocka 
    VALUES('5497','U éida','Staras 25');
INSERT INTO Pobocka
    VALUES('5764','Starovka','Rohova 41');
 INSERT INTO Pobocka 
    VALUES('3457','Nove Hrudy','Okruûa 50');
 INSERT INTO Pobocka
    VALUES('2587','Norka','Strohov 50');
INSERT INTO Pobocka
    VALUES('3521','Klaningrad','Okolska 41');

INSERT INTO Zamestnanec 
    VALUES('440706/0672', 'Peter', 'Dobrovsky', 'klientsky pracovnik', '0905294355','','5497');
INSERT INTO Zamestnanec 
    VALUES('582609/8592', 'Peter', 'Dobro', 'klientsky pracovnik', '0905456235','','5497');
INSERT INTO Zamestnanec 
    VALUES('140706/0697', 'Frantisek', 'Kahan', 'klientsky pracovnik', '0903698266','Bozetechova 6','5497');
INSERT INTO Zamestnanec 
    VALUES('440706/1234', 'Edmund', 'Litvak', 'manager poboËky', '0675297359','Purkynova 98','3457');
INSERT INTO Zamestnanec 
    VALUES('785607/5678', 'Matejka', 'Zuffa', 'manager banky', '0707294395','','3521');


INSERT INTO Klient 
    VALUES('440706/3314', 'Jozef', 'Kralavsky','0905294355','Purkynova 41');
INSERT INTO Klient
    VALUES('557701/5874', 'Peter', 'Grohan','0298426877','Purkynova 43');
INSERT INTO Klient
    VALUES('140706/0697', 'Frantisek', 'Kahan','0903698266','Purkynova 98');
INSERT INTO Klient
    VALUES('541806/2587', 'Sebastian', 'Nemcek','0901294395','Purkynova 74');
INSERT INTO Klient 
    VALUES('680415/3497', 'Emil', 'Hrusan','0675297359','Purkynova 35');


INSERT INTO Ucet
    VALUES('974685134','osobny','213',TO_DATE('14.04.2015','dd.mm.yyyy'),'440706/0672','440706/3314');
INSERT INTO Ucet
    VALUES('641328549','studentsky','124',TO_DATE('15.09.2015','dd.mm.yyyy'),'440706/0672','440706/3314');
INSERT INTO Ucet
    VALUES('649785342','firemny','452',TO_DATE('18.03.2016','dd.mm.yyyy'),'140706/0697','557701/5874');
INSERT INTO Ucet
    VALUES('438721905','studentsky','358',TO_DATE('15.02.2017','dd.mm.yyyy'),'140706/0697','557701/5874');
INSERT INTO Ucet
    VALUES('203649210','osobny','137',TO_DATE('11.11.2011','dd.mm.yyyy'),'140706/0697','557701/5874');

INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'680415/3497','641328549');
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'541806/2587','641328549');
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'440706/3314','649785342');

INSERT INTO Platobna_karta
    VALUES('1647520149753025','kreditna','974685134','440706/3314');
INSERT INTO Platobna_karta
    VALUES('9746201680526974','debetna','974685134','557701/5874');
INSERT INTO Platobna_karta
    VALUES('1647467502317998','kreditna','641328549','680415/3497');

INSERT INTO Prikaz
    VALUES(dept_prikaz.nextval,'Vypis',TO_DATE('11.11.2011 15:04:31','dd.mm.yyyy hh24:mi:ss'),'','','974685134','440706/3314','440706/0672');
INSERT INTO Prikaz
    VALUES(dept_prikaz.nextval,'Vklad',TO_DATE('20.12.2015 08:15:48','dd.mm.yyyy hh24:mi:ss'),'35','','974685134','440706/3314','440706/0672');
INSERT INTO Prikaz
    VALUES(dept_prikaz.nextval,'Vyber',TO_DATE('14.11.2000 16:45:31','dd.mm.yyyy hh24:mi:ss'),'28,80','','438721905','557701/5874','440706/1234');
INSERT INTO Prikaz
    VALUES(dept_prikaz.nextval,'Prevod',TO_DATE('14.11.2000 16:45:31','dd.mm.yyyy hh24:mi:ss'),'28,90','455684258','438721905','557701/5874','440706/0672');
    
INSERT INTO Sluzba
    VALUES(dept_sluzba.nextval,'IBank','438721905');
INSERT INTO Sluzba
    VALUES(dept_sluzba.nextval,'SmartBank','438721905');
INSERT INTO Sluzba
    VALUES(dept_sluzba.nextval,'PaidBack','203649210');
INSERT INTO Sluzba
    VALUES(dept_sluzba.nextval,'Goldcard','974685134');


-----------------------------------------------------------------------
--                3 Selecty / 2 Tabulky
--1.  KtorÌ zamestnanci pracuju na poboËke s ID = 5497 (poboËka U ûida)
--2.  KtorÌ zamestnanci uû zaloûili nejak˝ ˙Ëet
--3.  Ktor˝ zamestnanec zaloûil ˙Ëet 974685134
-----------------------------------------------------------------------
SELECT DISTINCT Z.meno, Z.priezvysko
    FROM Zamestnanec Z, Pobocka P
    WHERE Z.ID_Pobocky=P.ID_Pobocky AND P.ID_Pobocky='5497';

SELECT DISTINCT Z.meno, Z.priezvysko
    FROM Zamestnanec Z, Ucet U
    WHERE Z.ID_Zamestnanca=U.ID_ZAMESTNANCA;

SELECT DISTINCT Z.meno, Z.priezvysko
    FROM Zamestnanec Z, Ucet U
    WHERE Z.ID_Zamestnanca=U.ID_ZAMESTNANCA AND U.CISLO_UCTU = '974685134';
-----------------------------------------------------------------------
--                2 Selecty / 3 Tabulky
--1.  ⁄Ëty zaloûenÈ zamestnancom 140726/0697 (Frantiöek Kahan)
--    s aktÌvnou sluûbou IBank
--2.  Klienti (ktorÌ vlastnia) ˙Ëet zaloûen˝ zamestnancom 140726/0697
-----------------------------------------------------------------------
SELECT DISTINCT U.CISLO_UCTU
    FROM Zamestnanec Z, Ucet U, SLUZBA S
    WHERE Z.ID_Zamestnanca=U.ID_ZAMESTNANCA AND Z.ID_ZAMESTNANCA = '140706/0697'
          AND S.NAZOV = 'IBank' AND S.CISLO_UCTU = U.CISLO_UCTU;
          
SELECT DISTINCT K.MENO,K.PRIEZVYSKO
    FROM Zamestnanec Z, Ucet U, Klient K
    WHERE Z.ID_Zamestnanca=U.ID_ZAMESTNANCA AND Z.ID_ZAMESTNANCA = '140706/0697'
          AND U.ID_KLIENTA = K.ID_KLIENTA;
          
-----------------------------------------------------------------------
--                2 Selecty (GROUP BY a agregacna funkcia)
--1.  Kolko uctov zaloûili jednotlivÌ zamestnanci v prÌpade,
--    ûe uû nejakÌ zaloûili
--2.  Vsetci disponenti ˙Ëtu 641328549
-----------------------------------------------------------------------
SELECT DISTINCT Z.MENO,Z.PRIEZVYSKO,COUNT(*) UCTOV_ZALOZIL
    FROM Zamestnanec Z, Ucet U
    WHERE Z.ID_Zamestnanca=U.ID_ZAMESTNANCA
    GROUP BY Z.MENO,Z.PRIEZVYSKO;

SELECT DISTINCT K.MENO, K.PRIEZVYSKO
    FROM Disponent D, Ucet U, Klient K
    WHERE D.ID_KLIENTA = K.ID_KLIENTA AND D.CISLO_UCTU = U.CISLO_UCTU
          AND U.CISLO_UCTU = '641328549'
    GROUP BY K.MENO,K.PRIEZVYSKO;   

-----------------------------------------------------------------------
--                2 Selecty (IN a EXIST)
--1.  KtorÌ zamestnanci uû vykonali prÌkaz vyber a vklad
--2.  KtorÌ zamestnanci nikdy nezaloûili ˙Ëet a z·roveÚ nepracuj˙
--    na poboËke 5497(U ûida)
-----------------------------------------------------------------------
SELECT DISTINCT Z.MENO,Z.PRIEZVYSKO
    FROM Zamestnanec Z, Prikaz P
    WHERE P.ID_Zamestnanca = Z.ID_Zamestnanca
          AND P.typ_prikazu in ('Vyber','Vklad');

SELECT DISTINCT Z.MENO,Z.PRIEZVYSKO,Z.POZICIA
    FROM Zamestnanec Z, Pobocka P
    WHERE P.ID_POBOCKY not in ('5497') AND Z.ID_POBOCKY = P.ID_POBOCKY
          AND NOT EXISTS (SELECT *
                              FROM Ucet U
                              WHERE U.ID_ZAMESTNANCA = Z.ID_ZAMESTNANCA);
                              

-----------------------------------------------------------------------
--                  Triggery/Procedury
--  1. PROCED⁄RA na kontrolu spr·vnosti rodnÈho ËÌsla, ktor· mala
--     byù pÙvodne trigger. Avöak rodnÈ ËÌslo obsahuje ako klient tak aj zamestnanec
--     preto som sa rozhodol vytvoriù proced˙ru ktor· je volan· z dvoch rÙznych triggerov
--     pre 2 tabulky.
--  2. TRIGGER Overuje spr·vnosù rodnÈho ËÌsla zamestnanca 
--  3. TRIGGER Overuje spr·vnosù rodnÈho ËÌsla klienta
--  4. TRIGGER V prÌpade ûe je pk pri prÌkaze NULL nahradÌ ho nasledovnou generovanou hodnotou
--  5. PROCED⁄RA na z·klade rodnÈho ËÌsla klienta doch·dza k odstr·neniu vöetk˝ch disponentov
--     vr·tane nimi vlastnen˝ch platobn˝ch kariet k danÈmu ˙Ëtu
--  6. PROCED⁄RA vypÌöe na dbms output vöetk˝ch klientov ktor˝ kedy akokoævek
--     manipulovali Ëi vlastnili Ëi disponovali zadan˝m ˙Ëtom
-----------------------------------------------------------------------


CREATE OR REPLACE PROCEDURE rod_cislo (r_cislo CHAR)
AS
BEGIN
  DECLARE
    rok NUMBER(2);
    den NUMBER(2);
    mesiac NUMBER(2);
    por NUMBER(4);
  BEGIN
    rok := TO_NUMBER(substr(r_cislo , 1, 2));
    den   := TO_NUMBER(substr(r_cislo , 3, 2));
    mesiac := TO_NUMBER(substr(r_cislo , 5, 2));
    por := TO_NUMBER(substr(r_cislo , 8, 4));
  

    if (den>50)
    then    den := den - 50;
    end if;

    if (31<den) OR (den<1)
    then  raise_application_error(-20001,'wrong ID number');
    end if;

    if(mesiac<1) OR (mesiac>12)
    then  raise_application_error(-20001,'wrong ID number');
    end if;
  END;
END;
/

CREATE OR REPLACE TRIGGER r_cislo_Z
BEFORE INSERT OR UPDATE OF ID_Zamestnanca ON Zamestnanec
FOR EACH ROW
DECLARE
  r_cislo Zamestnanec.ID_Zamestnanca%TYPE;
BEGIN
  r_cislo := :NEW.ID_Zamestnanca;
  rod_cislo(r_cislo);  
END r_cislo_Z;
/

CREATE OR REPLACE TRIGGER r_cislo_K
BEFORE INSERT OR UPDATE OF ID_klienta ON Klient
FOR EACH ROW
DECLARE
  r_cislo Klient.ID_klienta%TYPE;
BEGIN
  r_cislo := :NEW.ID_klienta;
  rod_cislo(r_cislo);  
END r_cislo_K;
/

/*        --testy k triggrom na rodnÈ ËÌslo
--  //  CHYBNY DATUM V R.Ë. aj klienta aj zamestnanca c1
--  c1 vûdy vyvol· system fault s id -20001 'wrong ID number'
--  c1
INSERT INTO Zamestnanec       
    VALUES('788612/5678', 'Roman', 'Roxor', 'manager banky', '0707294395','','3521');
SELECT * FROM Zamestnanec;
--  c2
INSERT INTO Zamestnanec       
    VALUES('781612/5678', 'Robo', 'Rex', 'manager banky', '0707294395','','3521');
SELECT * FROM Zamestnanec;
--  c1
INSERT INTO Klient 
    VALUES('680411/3497', 'Jozo', 'Peso','0675297359','Purkynova 35');
SELECT * FROM Klient;    
--  c2
INSERT INTO Klient 
    VALUES('680415/3497', 'Mato', 'Prasil','0675297359','Purkynova 35');
SELECT * FROM Klient;    

*/

CREATE OR REPLACE TRIGGER trig_prikaz 
BEFORE INSERT ON Prikaz 
FOR EACH ROW
WHEN (new.pk IS NULL)
BEGIN
  :new.pk := dept_prikaz.NEXTVAL;
END;
/

INSERT INTO Prikaz (typ_prikazu,datum_vykonania,cislo_uctu,ID_Klienta,ID_Zamestnanca)
    VALUES('Vypis',TO_DATE('11.11.2011 15:04:31','dd.mm.yyyy hh24:mi:ss'),'974685134','440706/3314','440706/0672');
SELECT * FROM Prikaz; -- posledn˝ z·pis je v˝pis s vygenerovan˝m ËÌslom

CREATE OR REPLACE PROCEDURE del_disponents (r_cislo char)
IS
  CURSOR odstran IS SELECT * from Disponent 
  where Disponent.cislo_uctu in (SELECT cislo_uctu FROM Ucet WHERE ID_klienta=r_cislo);
  trow odstran%ROWTYPE;
BEGIN
  OPEN odstran;
  loop
    fetch odstran into trow;
    EXIT WHEN odstran%NOTFOUND;
    DELETE FROM Disponent WHERE (trow.cislo_uctu = Disponent.cislo_uctu);
    DELETE FROM Platobna_karta WHERE (trow.ID_klienta = Platobna_karta.ID_klienta 
                                      AND trow.cislo_uctu in (SELECT cislo_uctu FROM Ucet WHERE ID_klienta=r_cislo));
  end loop;
END del_disponents;
/


--  DÙjde k odstr·neniu 2 Disponentov a jednej platobnej karty nav‰zuj˙cich
--  na klienta, ktor˝ ûiada o zruöenie vöetk˝ch disponentov
SELECT * FROM Disponent;
SELECT * FROM Platobna_karta;
EXEC del_disponents('440706/3314');
SELECT * FROM Disponent;
SELECT * FROM Platobna_karta;

CREATE OR REPLACE PROCEDURE everything_for_acc(ucet NUMBER)
IS
  CURSOR zaznam IS SELECT * FROM Klient 
  WHERE ID_klienta in (SELECT ID_klienta FROM Disponent WHERE cislo_uctu = ucet
                       UNION SELECT ID_klienta FROM Platobna_karta WHERE cislo_uctu=ucet
                       UNION SELECT ID_klienta FROM Prikaz WHERE cislo_uctu=ucet
                       UNION SELECT ID_klienta FROM Ucet WHERE cislo_uctu=ucet);           
  trow zaznam%ROWTYPE;
BEGIN
   open zaznam;
      loop
        fetch zaznam into trow;
        exit when zaznam%NOTFOUND;
        dbms_output.put_line(trow.ID_klienta || ' ' || trow.meno || ' ' || trow.priezvysko);
      end loop;
END everything_for_acc;
/

-- zistÌ klientov ktor˝ manipulovali s ˙Ëtom '974685134'
EXEC everything_for_acc('974685134');



-----------------------------------------------------------------------
--                  EXPLAIN PLAN
-----------------------------------------------------------------------

--pomocne inserty ... neostava vela zaznamov v tabulke Disponent
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'680415/3497','641328549');
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'557701/5874','641328549');
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'140706/0697','641328549');
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'541806/2587','641328549');
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'680415/3497','974685134');
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'557701/5874','974685134');
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'541806/2587','203649210');
INSERT INTO Disponent 
    VALUES(dept_disp.nextval,'680415/3497','203649210');


EXPLAIN PLAN FOR
  SELECT Ucet.ID_KLIENTA , Ucet.cislo_uctu, COUNT(Disponent.ID_zaznam) AS pocet_disponentov
  FROM Ucet, Disponent
  WHERE Ucet.cislo_uctu = Disponent.cislo_uctu
  GROUP BY Ucet.cislo_uctu, Ucet.ID_klienta;
  
SELECT plan_table_output
from table(dbms_xplan.display());

create index dispo on Disponent(cislo_uctu);

EXPLAIN PLAN FOR
  SELECT Ucet.ID_KLIENTA , Ucet.cislo_uctu, COUNT(Disponent.ID_zaznam) AS pocet_disponentov
  FROM Ucet, Disponent
  WHERE Ucet.cislo_uctu = Disponent.cislo_uctu
  GROUP BY Ucet.cislo_uctu, Ucet.ID_klienta;
  
SELECT plan_table_output
from table(dbms_xplan.display());

/*        --Pouûit˝ SELECT(test)
            
  SELECT Ucet.ID_KLIENTA , Ucet.cislo_uctu, COUNT(Disponent.ID_zaznam) AS pocet_disponentov
    FROM Ucet, Disponent
    WHERE Ucet.cislo_uctu = Disponent.cislo_uctu
    GROUP BY Ucet.cislo_uctu, Ucet.ID_klienta;
*/


GRANT ALL ON Zamestnanec TO xpobis00;
GRANT ALL ON Klient TO xpobis00;
GRANT ALL ON Ucet TO xpobis00;
GRANT ALL ON Platobna_karta TO xpobis00;
GRANT ALL ON Prikaz TO xpobis00;
GRANT ALL ON Sluzba TO xpobis00;
GRANT ALL ON Pobocka TO xpobis00;
GRANT ALL ON Disponent TO  xpobis00;


-----------------------------------------------------------------------
--                 MATERIALIZED VIEW
-----------------------------------------------------------------------

create materialized view log on Klient with rowid;
create materialized view log on Ucet with rowid;

drop MATERIALIZED view Ucty;
create materialized view Ucty
  nologging
  cache
  build immediate
  refresh fast on commit
  enable query rewrite
  AS
    SELECT Klient.ID_klienta, Klient.meno, Klient.priezvysko, Ucet.cislo_uctu, Klient.rowid AS K_rowid, Ucet.rowid AS U_rowid 
    FROM Ucet,Klient
    WHERE Ucet.ID_klienta = Klient.ID_klienta;
SELECT * FROM Ucty;

/*      sp˙öùa druh˝ Ëlen t˝mu
create materialized view log on xgrofc00.Klient with rowid;
create materialized view log on xgrofc00.Ucet with rowid;

create materialized view Ucty
  nologging
  cache
  build immediate
  refresh fast on commit
  enable query rewrite
  AS
    SELECT xgrofc00.Klient.ID_klienta, xgrofc00.Klient.meno, xgrofc00.Klient.priezvysko, xgrofc00.Ucet.cislo_uctu, xgrofc00.Klient.rowid AS K_rowid, xgrofc00.Ucet.rowid AS U_rowid 
    FROM xgrofc00.Ucet, xgrofc00.Klient
    WHERE xgrofc00.Ucet.ID_klienta = xgrofc00.Klient.ID_klienta;
GRANT ALL ON Ucty to xgrofc00;
SELECT * FROM Ucty;
*/
COMMIT;
