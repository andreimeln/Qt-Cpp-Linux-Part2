create table status (
	iid serial not null primary key,
	code char(3) not null unique,
	title text
);

insert into status (iid, code, title)
values 	(-1, 'VYD','Vydano'),
	(-2, 'PER','V pereplyote'),
	(-3, 'REZ','Zarezervirovano');

alter table books
	add column 
	rid_status int references status(iid);
