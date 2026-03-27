Wokwi: https://wokwi.com/projects/456396662970036225

# Design Overview

## Architecture

The project is organized into modular components that separate concerns:

| Module | Responsibility |
|--------|----------------|
| `billboard.c` | Main controller - manages company list, selects ads, runs the loop |
| `company.c` | Company data and weighted random selection |
| `ad.c` | Ad display logic (animations, scrolling, blinking) |
| `lcd.c` | Low-level LCD hardware control |

## Struct Design

### `struct Billboard`
- Owns the list of companies (dynamic array)
- Tracks `active_company` to avoid showing the same company twice in a row
- Uses opaque struct pattern - header only declares struct, implementation is hidden

### `struct Company`
- Contains advertising balance (`ad_balance`) that decreases over time
- Stores ads as a dynamic array (`ad_collection`)
- Uses pointer to string (`char *`) for name - avoids copying, saves RAM on AVR
- `company_type` distinguishes OWNER (fallback when no budget) from CUSTOMER

### `struct Ad`
- Value-type struct (not pointer) - small enough to copy cheaply
- Contains display parameters: text, animation type, time-based rules

### CompanySelector Pattern
Weighted random selection uses a range-based approach:
- Each company gets a range proportional to its balance
- Random number determines which range (and thus which company) wins
- More balance = bigger range = higher chance of selection

## Design Choices

1. **Opaque struct for Billboard** - Hides implementation, allows internal changes without breaking ABI
2. **Pointer-based strings** - AVR has limited RAM; string literals stay in flash, only pointers are copied
3. **Value-type Ad struct** - Small enough (3 fields) that copying is cheaper than indirection
4. **Dynamic arrays with realloc** - Companies are added/removed at runtime, flexibility over fixed-size arrays
5. **Owner fallback** - Ensures billboard always has something to display when all customers run out of balance
