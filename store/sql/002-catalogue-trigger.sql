create or replace function adjust_catalogue_item() returns trigger as $$
DECLARE
  R int;
BEGIN
  if NEW.rid_parent is NULL then /* NEW used for the record that is being inserted */
    NEW.alevel := 0;
  else
    select alevel
      into strict R      /* strict means this select should return strictly one record */
      from catalogue
      where iid = NEW.rid_parent;
    NEW.alevel := R + 1;
  end if;
  if NEW.valid_from is NULL then
    NEW.valid_from = CURRENT_TIMESTAMP;
  end if;
  return NEW;
END;
$$
  language plpgsql
  security definer;

create trigger adjust_catalogue_item_trigger
  before insert or update
  on catalogue
  for each row
  execute procedure adjust_catalogue_item();

