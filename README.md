# Documentation

## Files and functionality overview
| File | Functionality |
|------|---------------|
| billboard.c | Main file for the system which preps and running the main program loop - handles & selects company ads|
| company.c | Company data and weighted random select |
| ad.c | Display logics and handles ad_rules to display different types of ad (blinking, scrolling, static) |
| millis_helper.c | Determines if billboard is on odd/even minute cycle |

## Structs
### struct Billboard
- Encapsulated
- Owns the list of companies and keeps track of how many companies have been added with num_companies attribute   
- Tracks active_company to avoid showing the same company twice in a row.




Wokwi: 
https://wokwi.com/projects/456396662970036225



# TODO
- [x] Real struct of billboard. company, ads blinking animation, 
- [x] Balance logics & random logics
    - [x] Filter companies which does not have the balance
    - [x] Add logic to add more weight to companies which have more balance and give them a higher chance to get picked.
- [x] Create company function.
- [x] declare function in headerfiles (billboard.h)
- [x] break out function into separate files (company.h / ad.h etc)
- [x] Refactor billboard



