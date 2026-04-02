# Documentation
Wokwi "digital twin": 
https://wokwi.com/projects/456396662970036225


## Files and functionality overview
| File | Functionality |
|------|---------------|
| billboard.c | Main file for the system which preps and running the main program loop - handles & selects company ads|
| company.c | Company data and weighted random select |
| ad.c | Display logics and handles ad_rules to display different types of ad (blinking, scrolling, static) |
| millis_helper.c | Determines if billboard is on odd/even minute cycle |

## Structs
### struct Billboard
- Encapsulated type
- Owns the list of companies and keeps track of how many companies have been added with num_companies attribute   
- Tracks active_company to avoid showing the same company twice in a row.

### struct Company
- Contains ad_balance for weighted selection.
- Stores array of ads.
- Has enum company_type to determine if company is a customer or owner of the billboard. Can be used for future to add diffrent type as "VIP" to handle usecaseslike discounts or whatever.

### struct Ad
- Contains ad_text whih will be displayed on the LCD.
- enum Animation options SCROLL, BLINK or NONE.
- enum AdStrategyRule to determine prefered ad selection (EVEN/ODD_MINUTES or random) if company has a specific CompanyAdStrategy.

### struct CompanySlot
- Contains company, range_min & range_max.
- Created temporarily to add a company with range to select a company randomly with with company balance taken into account. The greater ad_balance a compy has, the greater chance the company is to get randomly chosen in the CompanySelector. 

### struct CompanySelector
- Contains array of CompanySlot, num_companies and total_balance to make weighted selection possible from the CompanySlot array ranges. Used in function billboard_select_company().

## Possible improvements
### Memory allocation
In this project there is multiple place where malloc & realloc is being used. This could cause heap fragmentation and unpredictable execution times. Since the memory resources are limited it would be better to use stack allocation since the number of companies and ads are known at compile time. This would ensure deterministic memory access and reduce code complexity. 



# TODO
- [x] Real struct of billboard. company, ads blinking animation, 
- [x] Balance logics & random logics
    - [x] Filter companies which does not have the balance
    - [x] Add logic to add more weight to companies which have more balance and give them a higher chance to get picked.
- [x] Create company function.
- [x] declare function in headerfiles (billboard.h)
- [x] break out function into separate files (company.h / ad.h etc)
- [x] Refactor billboard



