# Argumenta: Command-Line Parsing That Doesn't Make You Want To Quit

Look, you're writing a command-line program. I know you are. And you need to parse arguments. You know, the stuff users type after your program name: flags, options, file names, whatever.

Here's what you're thinking: "I'll just loop through `argv` and check each string manually. How hard could it be?"

Real hard. That's how hard.

You're gonna have flags like `-v` and `--verbose`. You're gonna have options that take values like `--output file.txt`. You're gonna support `--config=settings.ini` because users expect that. You're gonna have positional arguments. Some required, some optional. And you're gonna want to generate a help message automatically instead of maintaining it by hand in three different places.

Or you could use **argumenta** and not hate your life.

**Argumenta** - Latin for "arguments" or "proofs." Here it means "command-line arguments." Yeah, same word in both languages. We got lucky.

This library gives you:
- Flags (boolean switches like `-v` or `--verbose`)
- Options (values like `-o output.txt` or `--output=output.txt`)
- Positional arguments (file names, etc.)
- Short forms (`-v`) and long forms (`--verbose`)
- Automatic help text generation
- Error handling with descriptive messages
- Piscina-based memory management

## How Command-Line Arguments Work (The Basics)

When you run a program like this:

```
$ myprogram -v --output=file.txt input.txt
```

The operating system gives you an `argc` (argument count) and `argv` (argument vector - array of strings):

```c
argc = 4
argv[0] = "myprogram"  /* Program name */
argv[1] = "-v"
argv[2] = "--output=file.txt"
argv[3] = "input.txt"
```

You loop through `argv[1]` to `argv[argc-1]` and figure out what each one means.

Conventions:
- Arguments starting with `-` are flags or options
- Short form: single dash + single letter (`-v`, `-h`)
- Long form: double dash + word (`--verbose`, `--help`)
- Options take values: either next argument (`-o file.txt`) or with equals (`--output=file.txt`)
- Arguments without dashes are positional (file names, commands, etc.)

Your job: parse all this, validate it, make it available to your program logic.

This library's job: do that for you so you can get back to actual work.

## Latin Crash Course (Getting Old Yet?)

More Latin because apparently this is our life now:

- **argumenta** = arguments, proofs
- **parser** = parser (English loanword)
- **fructus** = fruit, result (what you get after parsing)
- **vexillum** = flag, banner (like `-v` or `--verbose`)
- **optio** = option, choice
- **positionalis** = positional
- **addere** = to add
- **creare** = to create
- **parsere** = to parse
- **conari** = to try, to attempt
- **obtinere** = to obtain, to get
- **habet** = it has (from *habere*, to have)
- **numerus** = number
- **descriptio** = description
- **exemplum** = example (plural: *exempla*)
- **auxilium** = help, aid
- **imprimere** = to print
- **nuntius** = message
- **erroris** = of error (genitive)
- **necessarius** = necessary, required
- **brevis** = short, brief
- **longus** = long
- **titulus** = title, name

So `argumenta_addere_vexillum()` = "arguments_add_flag". You're learning Latin whether you like it or not.

## Creating A Parser

First step: create a parser object. This holds all your argument definitions.

```c
Piscina* piscina = piscina_generare_dynamicum("args", 4096);

ArgumentaParser* parser = argumenta_creare(piscina);
si (!parser)
{
    imprimere("Failed to create parser\n");
    exire(I);
}
```

**`argumenta_creare(piscina)`** - "arguments_create"

Creates an ArgumentaParser allocated from your piscina. Returns `NIHIL` on failure.

The parser starts empty. You add definitions to it (flags, options, positional args), then use it to parse actual command-line arguments.

## Defining Flags

Flags are boolean switches. They're either present or not. No value.

```c
argumenta_addere_vexillum(parser, "-v", "--verbose", "Enable verbose output");
argumenta_addere_vexillum(parser, "-q", "--quiet", "Suppress output");
argumenta_addere_vexillum(parser, "-h", "--help", "Show help message");
```

**`argumenta_addere_vexillum(parser, nomen_brevis, nomen_longus, descriptio)`**

- `parser` - your ArgumentaParser
- `nomen_brevis` - short form (like `"-v"`) - can be `NIHIL` if you only want long form
- `nomen_longus` - long form (like `"--verbose"`) - can be `NIHIL` if you only want short form
- `descriptio` - help text describing what this flag does

At least one of `nomen_brevis` or `nomen_longus` must be non-NULL.

Users can use either form. Both set the flag:

```
$ myprogram -v        # flag is set
$ myprogram --verbose # also sets the flag
```

After parsing, you check if the flag was present:

```c
si (argumenta_habet_vexillum(fructus, "-v"))
{
    imprimere("Verbose mode enabled\n");
}
```

You can check using either the short or long form. They're equivalent.

## Defining Options

Options are like flags but they take a value.

```c
argumenta_addere_optionem(parser, "-o", "--output", "Output file path");
argumenta_addere_optionem(parser, "-c", "--config", "Config file path");
argumenta_addere_optionem(parser, NIHIL, "--log-level", "Logging level (debug, info, warn, error)");
```

**`argumenta_addere_optionem(parser, nomen_brevis, nomen_longus, descriptio)`**

Same parameters as `addere_vexillum`, but options expect a value.

Users provide the value in two ways:

**Space-separated**:
```
$ myprogram -o output.txt --config settings.ini
```

**Equals syntax** (long form only):
```
$ myprogram --output=output.txt --config=settings.ini
```

After parsing, you retrieve the value:

```c
chorda output = argumenta_obtinere_optionem(fructus, "-o", piscina);
si (output.mensura > ZEPHYRUM)
{
    imprimere("Output file: %.*s\n", output.mensura, output.datum);
}
alioquin
{
    imprimere("No output file specified\n");
}
```

If the option wasn't provided, you get back a `chorda` with `mensura == 0` and `datum == NIHIL`.

## Defining Positional Arguments

Positional arguments don't have dashes. They're identified by position.

```c
argumenta_addere_positionalem(parser, "input", "Input file to process", VERUM);
argumenta_addere_positionalem(parser, "output", "Output file (optional)", FALSUM);
```

**`argumenta_addere_positionalem(parser, titulus, descriptio, necessarius)`**

- `titulus` - name for the argument (used in help text)
- `descriptio` - what it's for
- `necessarius` - `VERUM` if required, `FALSUM` if optional

The order you add them matters. First positional you add is position 0, second is position 1, etc.

User provides them in order:

```
$ myprogram input.txt output.txt
             ^         ^
          position 0   position 1
```

After parsing, retrieve by index:

```c
i32 num_positionals = argumenta_numerus_positionalium(fructus);
per (i32 i = ZEPHYRUM; i < num_positionals; i++)
{
    chorda arg = argumenta_obtinere_positionalem(fructus, i, piscina);
    imprimere("Positional %d: %.*s\n", i, arg.mensura, arg.datum);
}
```

Or if you know the positions:

```c
chorda input = argumenta_obtinere_positionalem(fructus, ZEPHYRUM, piscina);
si (input.mensura == ZEPHYRUM)
{
    imprimere("Error: input file required\n");
    exire(I);
}
```

**Required vs Optional**: If you mark a positional as required (`necessarius = VERUM`), parsing fails if the user doesn't provide it.

## Mixing Flags, Options, and Positionals

You can mix all three types. The parser figures it out:

```c
argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose output");
argumenta_addere_optionem(parser, "-o", "--output", "Output file");
argumenta_addere_positionalem(parser, "input", "Input file", VERUM);
```

User can provide them in any order:

```
$ myprogram -v --output=out.txt input.txt
$ myprogram input.txt -v --output=out.txt
$ myprogram --output=out.txt input.txt -v
```

All three are equivalent. The parser collects flags and options, then positionals in the order they appear (ignoring flags/options).

After parsing:
- Check flags with `argumenta_habet_vexillum()`
- Get option values with `argumenta_obtinere_optionem()`
- Get positionals with `argumenta_obtinere_positionalem()`

## Parsing Arguments

Once you've defined everything, parse the actual command-line arguments:

```c
ArgumentaFructus* fructus = argumenta_parsere(parser, argc, argv);
```

**`argumenta_parsere(parser, argc, argv)`** - "arguments_parse"

- `parser` - your parser with all the definitions
- `argc` - argument count from `main()`
- `argv` - argument vector from `main()`

Returns an ArgumentaFructus (parse result) on success, or calls `exit(1)` on failure (after printing an error message).

**This function exits on error**. If you want to handle errors yourself, use the safe version:

```c
ArgumentaFructus* fructus = argumenta_conari_parsere(parser, argc, argv);
si (!fructus)
{
    /* Parse failed */
    argumenta_imprimere_errorem(parser);
    redde I;
}
```

**`argumenta_conari_parsere()`** - "arguments_try_parse"

Returns `NIHIL` on failure instead of exiting. You check the error with `argumenta_imprimere_errorem()`.

### What Counts As An Error?

- Unknown flag or option (not defined)
- Option missing its value (`--output` with no file after it)
- Flag with a value (`--verbose=true` when `--verbose` is a flag)
- Required positional arguments missing

## Checking Flags

After parsing, check if a flag was set:

```c
si (argumenta_habet_vexillum(fructus, "-v"))
{
    verbose_mode = VERUM;
}

si (argumenta_habet_vexillum(fructus, "--debug"))
{
    debug_mode = VERUM;
}
```

**`argumenta_habet_vexillum(fructus, titulus)`** - "arguments_has_flag"

Returns `VERUM` if the flag was present, `FALSUM` if not.

You can check using either the short form (`"-v"`) or long form (`"--verbose"`). Both work because they refer to the same flag.

## Getting Option Values

Retrieve an option's value:

```c
chorda output = argumenta_obtinere_optionem(fructus, "--output", piscina);
si (output.mensura > ZEPHYRUM)
{
    /* User provided --output */
    open_file(output);
}
alioquin
{
    /* User didn't provide --output, use default */
    output = chorda_ex_literis("default.txt", piscina);
}
```

**`argumenta_obtinere_optionem(fructus, titulus, piscina)`** - "arguments_obtain_option"

- `fructus` - parse result
- `titulus` - option name (short or long form)
- `piscina` - arena to allocate the returned chorda from

Returns a `chorda` with the value. If the option wasn't provided, `mensura == 0` and `datum == NIHIL`.

Check using either the short or long form. Both work.

## Getting Positional Arguments

Retrieve positionals by index:

```c
i32 count = argumenta_numerus_positionalium(fructus);
si (count < I)
{
    imprimere("Error: input file required\n");
    exire(I);
}

chorda input = argumenta_obtinere_positionalem(fructus, ZEPHYRUM, piscina);
imprimere("Processing: %.*s\n", input.mensura, input.datum);
```

**`argumenta_numerus_positionalium(fructus)`** - "arguments_number_of_positionals"

Returns how many positional arguments the user provided.

**`argumenta_obtinere_positionalem(fructus, index, piscina)`** - "arguments_obtain_positional"

- `fructus` - parse result
- `index` - which positional (0-indexed)
- `piscina` - arena for the returned chorda

Returns a `chorda` with the positional argument value. If the index is out of range, `mensura == 0` and `datum == NIHIL`.

## Adding Help Text

You should provide a description of your program and usage examples:

```c
argumenta_ponere_descriptionem(parser,
    "mytool - Processes files and does stuff");

argumenta_addere_exemplum(parser, "mytool -v input.txt");
argumenta_addere_exemplum(parser, "mytool --output=out.txt input.txt");
argumenta_addere_exemplum(parser, "mytool -v --config=settings.ini input.txt output.txt");
```

**`argumenta_ponere_descriptionem(parser, descriptio)`** - "arguments_set_description"

Sets the program description shown at the top of help text.

**`argumenta_addere_exemplum(parser, exemplum)`** - "arguments_add_example"

Adds a usage example. You can add multiple examples.

## Printing Help

Generate and print help text:

```c
si (argumenta_habet_vexillum(fructus, "--help"))
{
    argumenta_imprimere_auxilium(parser);
    exire(ZEPHYRUM);
}
```

**`argumenta_imprimere_auxilium(parser)`** - "arguments_print_help"

Prints a formatted help message to stdout:

```
mytool - Processes files and does stuff

USUS:
    [OPTIONES] <input> [output]

OPTIONES:
    -v, --verbose
        Enable verbose output

    -o, --output <valor>
        Output file path

    -h, --help
        Show help message

ARGUMENTA:
    <input>
        Input file to process

    <output> (optionalis)
        Output file

EXEMPLA:
    mytool -v input.txt
    mytool --output=out.txt input.txt
    mytool -v --config=settings.ini input.txt output.txt
```

The help is generated automatically from your definitions. You don't maintain it separately.

**Format**:
- Required positionals: `<name>`
- Optional positionals: `[name]`
- Options that take values: `--option <valor>`
- Flags: just the flag name

## Printing Errors

When parsing fails (using `conari_parsere`), print the error:

```c
ArgumentaFructus* fructus = argumenta_conari_parsere(parser, argc, argv);
si (!fructus)
{
    argumenta_imprimere_errorem(parser);
    exire(I);
}
```

**`argumenta_imprimere_errorem(parser)`** - "arguments_print_error"

Prints the error message to stderr:

```
Errore: Optio ignota: --unknown
Usa --help pro auxilio
```

Or:

```
Errore: Optio --output requirit valorem
Usa --help pro auxilio
```

The error messages are in Latin because everything is in Latin. If you need English, translate them yourself.

## Complete Example

Putting it all together:

```c
s32 principale(i32 argc, constans character** argv)
{
    Piscina*          piscina;
    ArgumentaParser*  parser;
    ArgumentaFructus* args;
    chorda            input;
    chorda            output;
    b32               verbose;

    /* Setup */
    piscina = piscina_generare_dynamicum("app", 8192);
    parser  = argumenta_creare(piscina);

    /* Define arguments */
    argumenta_ponere_descriptionem(parser,
        "fileproc - Process files and output results");

    argumenta_addere_vexillum(parser, "-v", "--verbose",
        "Enable verbose logging");
    argumenta_addere_vexillum(parser, "-h", "--help",
        "Show this help message");

    argumenta_addere_optionem(parser, "-o", "--output",
        "Output file (default: stdout)");
    argumenta_addere_optionem(parser, "-c", "--config",
        "Configuration file");

    argumenta_addere_positionalem(parser, "input",
        "Input file to process", VERUM);

    argumenta_addere_exemplum(parser, "fileproc -v input.txt");
    argumenta_addere_exemplum(parser, "fileproc --output=out.txt input.txt");

    /* Parse */
    args = argumenta_parsere(parser, argc, argv);

    /* Check for help */
    si (argumenta_habet_vexillum(args, "--help"))
    {
        argumenta_imprimere_auxilium(parser);
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    /* Get values */
    verbose = argumenta_habet_vexillum(args, "-v");
    input   = argumenta_obtinere_positionalem(args, ZEPHYRUM, piscina);
    output  = argumenta_obtinere_optionem(args, "--output", piscina);

    si (verbose)
    {
        imprimere("Verbose mode enabled\n");
    }

    si (output.mensura > ZEPHYRUM)
    {
        imprimere("Output file: %.*s\n", output.mensura, output.datum);
    }

    imprimere("Processing: %.*s\n", input.mensura, input.datum);

    /* ... do actual work ... */

    /* Cleanup */
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
```

## Memory Management

Everything comes from the piscina:
- The parser itself
- All argument definitions (strings are copied)
- The parse result
- Values returned by `obtinere_optionem()` and `obtinere_positionalem()`

When you destroy the piscina, everything is freed.

Typical pattern:

```c
Piscina* piscina = piscina_generare_dynamicum("app", 8192);

/* Create parser, parse args, use results */
ArgumentaParser* parser = argumenta_creare(piscina);
/* ... define arguments ... */
ArgumentaFructus* args = argumenta_parsere(parser, argc, argv);
/* ... use args ... */

/* Destroy piscina - everything freed */
piscina_destruere(piscina);
```

All strings (argument names, descriptions, values) are allocated from the piscina and freed when you destroy it.

## Common Patterns

### Pattern 1: Help Flag

Always add a help flag:

```c
argumenta_addere_vexillum(parser, "-h", "--help", "Show help message");

ArgumentaFructus* args = argumenta_parsere(parser, argc, argv);

si (argumenta_habet_vexillum(args, "--help"))
{
    argumenta_imprimere_auxilium(parser);
    exire(ZEPHYRUM);
}
```

### Pattern 2: Required Option With Default

Option is optional, but you provide a default:

```c
argumenta_addere_optionem(parser, "-o", "--output", "Output file");

chorda output = argumenta_obtinere_optionem(args, "--output", piscina);
si (output.mensura == ZEPHYRUM)
{
    output = chorda_ex_literis("default_output.txt", piscina);
}
```

### Pattern 3: Multiple Input Files

Use positionals for file lists:

```c
argumenta_addere_positionalem(parser, "files", "Input files", VERUM);

i32 num_files = argumenta_numerus_positionalium(args);
per (i32 i = ZEPHYRUM; i < num_files; i++)
{
    chorda file = argumenta_obtinere_positionalem(args, i, piscina);
    process_file(file);
}
```

### Pattern 4: Verbose Flag Controls Logging

```c
argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose output");

b32 verbose = argumenta_habet_vexillum(args, "-v");

si (verbose)
{
    imprimere("Starting processing...\n");
}

process_data();

si (verbose)
{
    imprimere("Done.\n");
}
```

### Pattern 5: Config File Option

```c
argumenta_addere_optionem(parser, "-c", "--config", "Config file");

chorda config_path = argumenta_obtinere_optionem(args, "--config", piscina);
si (config_path.mensura > ZEPHYRUM)
{
    load_config(config_path);
}
alioquin
{
    /* Use default config */
    load_default_config();
}
```

## Common Mistakes

### Mistake 1: Forgetting To Check Help Flag

```c
/* BAD */
ArgumentaFructus* args = argumenta_parsere(parser, argc, argv);
/* Forgot to check --help, so help flag does nothing */
```

Always check for help after parsing and exit early if it's set.

### Mistake 2: Wrong Order For Positional Definitions

```c
/* BAD - Definitions don't match usage */
argumenta_addere_positionalem(parser, "output", "Output file", FALSUM);
argumenta_addere_positionalem(parser, "input", "Input file", VERUM);

/* User types: myprogram input.txt output.txt */
/* But parser expects: myprogram output.txt input.txt */
```

Define positionals in the order users will provide them.

### Mistake 3: Not Checking If Option Was Provided

```c
/* BAD */
chorda output = argumenta_obtinere_optionem(args, "--output", piscina);
FILUM* f = fopen(output.datum, "w");  /* CRASH if output.datum is NIHIL */
```

Always check if `mensura > ZEPHYRUM` before using the value.

### Mistake 4: Using Parser After Piscina Destruction

```c
/* BAD */
Piscina* p = piscina_generare_dynamicum("app", 4096);
ArgumentaParser* parser = argumenta_creare(p);
ArgumentaFructus* args = argumenta_parsere(parser, argc, argv);

piscina_destruere(p);

/* Now parser and args are gone! */
si (argumenta_habet_vexillum(args, "-v"))  /* CRASH */
{
    /* ... */
}
```

Don't destroy the piscina until you're done with the parse results.

### Mistake 5: Defining Flag As Option

```c
/* BAD */
argumenta_addere_optionem(parser, "-v", "--verbose", "Verbose");

/* User types: myprogram -v */
/* Parser complains: "Optio -v requirit valorem" */
```

If it doesn't take a value, it's a flag. Use `addere_vexillum()`, not `addere_optionem()`.

### Mistake 6: Checking Wrong Name Form

```c
argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");

/* Later... */
si (argumenta_habet_vexillum(args, "verbose"))  /* Wrong! No dashes */
{
    /* This will never be true */
}
```

Include the dashes when checking: `"-v"` or `"--verbose"`, not `"verbose"`.

---

## API Reference

### Creation

#### `argumenta_creare()`

```c
ArgumentaParser* argumenta_creare(
    Piscina* piscina
);
```

Creates an argument parser.

**Parameters:**
- `piscina`: Arena allocator

**Returns:**
- Pointer to ArgumentaParser on success
- `NIHIL` if piscina is NULL or allocation fails

**Example:**
```c
ArgumentaParser* parser = argumenta_creare(piscina);
```

---

### Defining Arguments - Flags

#### `argumenta_addere_vexillum()`

```c
vacuum argumenta_addere_vexillum(
       ArgumentaParser* parser,
    constans character* nomen_brevis,
    constans character* nomen_longus,
    constans character* descriptio
);
```

Adds a flag definition.

**Parameters:**
- `parser`: Argument parser
- `nomen_brevis`: Short form (e.g., `"-v"`) or `NIHIL`
- `nomen_longus`: Long form (e.g., `"--verbose"`) or `NIHIL`
- `descriptio`: Help text or `NIHIL`

**Notes:**
- At least one of `nomen_brevis` or `nomen_longus` must be non-NULL
- Flags are boolean (present or not)
- Don't take values

**Example:**
```c
argumenta_addere_vexillum(parser, "-v", "--verbose", "Enable verbose output");
```

---

### Defining Arguments - Options

#### `argumenta_addere_optionem()`

```c
vacuum argumenta_addere_optionem(
       ArgumentaParser* parser,
    constans character* nomen_brevis,
    constans character* nomen_longus,
    constans character* descriptio
);
```

Adds an option definition (takes a value).

**Parameters:**
- `parser`: Argument parser
- `nomen_brevis`: Short form (e.g., `"-o"`) or `NIHIL`
- `nomen_longus`: Long form (e.g., `"--output"`) or `NIHIL`
- `descriptio`: Help text or `NIHIL`

**Notes:**
- Options require values
- Supports space-separated (`-o file.txt`) and equals syntax (`--output=file.txt`)

**Example:**
```c
argumenta_addere_optionem(parser, "-o", "--output", "Output file path");
```

---

### Defining Arguments - Positionals

#### `argumenta_addere_positionalem()`

```c
vacuum argumenta_addere_positionalem(
       ArgumentaParser* parser,
    constans character* titulus,
    constans character* descriptio,
                   b32  necessarius
);
```

Adds a positional argument definition.

**Parameters:**
- `parser`: Argument parser
- `titulus`: Name for help text
- `descriptio`: Help text or `NIHIL`
- `necessarius`: `VERUM` if required, `FALSUM` if optional

**Notes:**
- Order matters (first added = position 0)
- Required positionals must come before optional ones

**Example:**
```c
argumenta_addere_positionalem(parser, "input", "Input file", VERUM);
argumenta_addere_positionalem(parser, "output", "Output file", FALSUM);
```

---

### Help Text

#### `argumenta_ponere_descriptionem()`

```c
vacuum argumenta_ponere_descriptionem(
       ArgumentaParser* parser,
    constans character* descriptio
);
```

Sets program description for help text.

**Parameters:**
- `parser`: Argument parser
- `descriptio`: Program description

**Example:**
```c
argumenta_ponere_descriptionem(parser, "mytool - Does cool stuff");
```

---

#### `argumenta_addere_exemplum()`

```c
vacuum argumenta_addere_exemplum(
       ArgumentaParser* parser,
    constans character* exemplum
);
```

Adds a usage example for help text.

**Parameters:**
- `parser`: Argument parser
- `exemplum`: Example command line

**Example:**
```c
argumenta_addere_exemplum(parser, "mytool -v input.txt");
argumenta_addere_exemplum(parser, "mytool --output=out.txt file.txt");
```

---

### Parsing

#### `argumenta_parsere()`

```c
ArgumentaFructus* argumenta_parsere(
                 ArgumentaParser* parser,
                             i32  argc,
    constans character* constans* argv
);
```

Parses command-line arguments. Exits on error.

**Parameters:**
- `parser`: Parser with definitions
- `argc`: Argument count from `main()`
- `argv`: Argument vector from `main()`

**Returns:**
- Pointer to ArgumentaFructus (parse result)
- Never returns `NIHIL` (exits on error)

**Example:**
```c
ArgumentaFructus* args = argumenta_parsere(parser, argc, argv);
```

---

#### `argumenta_conari_parsere()`

```c
ArgumentaFructus* argumenta_conari_parsere(
                 ArgumentaParser* parser,
                             i32  argc,
    constans character* constans* argv
);
```

Parses command-line arguments. Returns `NIHIL` on error.

**Parameters:**
- `parser`: Parser with definitions
- `argc`: Argument count
- `argv`: Argument vector

**Returns:**
- Pointer to ArgumentaFructus on success
- `NIHIL` on error (check with `argumenta_imprimere_errorem()`)

**Example:**
```c
ArgumentaFructus* args = argumenta_conari_parsere(parser, argc, argv);
si (!args)
{
    argumenta_imprimere_errorem(parser);
    exire(I);
}
```

---

### Querying Results - Flags

#### `argumenta_habet_vexillum()`

```c
b32 argumenta_habet_vexillum(
    constans ArgumentaFructus* fructus,
           constans character* titulus
);
```

Checks if a flag was present.

**Parameters:**
- `fructus`: Parse result
- `titulus`: Flag name (short or long form)

**Returns:**
- `VERUM` if flag was present
- `FALSUM` if not present

**Example:**
```c
si (argumenta_habet_vexillum(args, "-v"))
{
    imprimere("Verbose mode\n");
}
```

---

### Querying Results - Options

#### `argumenta_obtinere_optionem()`

```c
chorda argumenta_obtinere_optionem(
    constans ArgumentaFructus* fructus,
           constans character* titulus,
                      Piscina* piscina
);
```

Gets an option's value.

**Parameters:**
- `fructus`: Parse result
- `titulus`: Option name (short or long form)
- `piscina`: Arena for returned chorda

**Returns:**
- `chorda` with value if option was provided
- `chorda` with `mensura == 0` if not provided

**Example:**
```c
chorda output = argumenta_obtinere_optionem(args, "--output", piscina);
si (output.mensura > ZEPHYRUM)
{
    imprimere("Output: %.*s\n", output.mensura, output.datum);
}
```

---

### Querying Results - Positionals

#### `argumenta_obtinere_positionalem()`

```c
chorda argumenta_obtinere_positionalem(
    constans ArgumentaFructus* fructus,
                          i32  index,
                      Piscina* piscina
);
```

Gets a positional argument by index.

**Parameters:**
- `fructus`: Parse result
- `index`: Position (0-indexed)
- `piscina`: Arena for returned chorda

**Returns:**
- `chorda` with value if index is valid
- `chorda` with `mensura == 0` if index out of range

**Example:**
```c
chorda input = argumenta_obtinere_positionalem(args, ZEPHYRUM, piscina);
si (input.mensura > ZEPHYRUM)
{
    imprimere("Input: %.*s\n", input.mensura, input.datum);
}
```

---

#### `argumenta_numerus_positionalium()`

```c
i32 argumenta_numerus_positionalium(
    constans ArgumentaFructus* fructus
);
```

Returns number of positional arguments provided.

**Parameters:**
- `fructus`: Parse result

**Returns:**
- Count of positional arguments

**Example:**
```c
i32 count = argumenta_numerus_positionalium(args);
imprimere("Got %d positional arguments\n", count);
```

---

### Help and Errors

#### `argumenta_imprimere_auxilium()`

```c
vacuum argumenta_imprimere_auxilium(
    constans ArgumentaParser* parser
);
```

Prints formatted help text to stdout.

**Parameters:**
- `parser`: Parser with definitions

**Example:**
```c
si (argumenta_habet_vexillum(args, "--help"))
{
    argumenta_imprimere_auxilium(parser);
    exire(ZEPHYRUM);
}
```

---

#### `argumenta_imprimere_errorem()`

```c
vacuum argumenta_imprimere_errorem(
    constans ArgumentaParser* parser
);
```

Prints error message to stderr.

**Parameters:**
- `parser`: Parser with error state

**Example:**
```c
ArgumentaFructus* args = argumenta_conari_parsere(parser, argc, argv);
si (!args)
{
    argumenta_imprimere_errorem(parser);
    exire(I);
}
```

---

## FAQ

### Q: Can I have both short and long forms for the same argument?

Yes. That's the whole point. Define both and users can use either one.

### Q: Can I have only a short form or only a long form?

Yes. Pass `NIHIL` for the one you don't want. At least one must be non-NULL.

### Q: What's the difference between a flag and an option?

Flags are boolean (present or not). Options take values. Use `addere_vexillum()` for flags, `addere_optionem()` for options.

### Q: Can users combine short flags like `-vq`?

No. This library doesn't support that. Each flag must be separate: `-v -q`.

### Q: What if I want to parse `--` to mean "everything after this is positional"?

This library doesn't support that. Anything without a leading dash is positional.

### Q: Can I have multiple values for one option (like `--input file1.txt file2.txt`)?

No. Options take one value. Use positional arguments for multiple files.

### Q: How do I make an option required?

You don't. Options are optional by definition. If it's required, use a positional argument.

### Q: What if the user provides the same flag twice?

It's still just set once. Doesn't matter how many times they type it.

### Q: What if the user provides the same option twice with different values?

The last one wins. No warning.

### Q: Can I parse arguments multiple times with the same parser?

Yes. Create the parser once, call `parsere()` multiple times if needed (though why would you?).

### Q: What's the maximum number of arguments I can define?

128 definitions (ARGUMENTA_MAXIMUS_DEFINITIONES). If you need more, you're doing something wrong.

### Q: What's the maximum number of examples I can add?

16 (ARGUMENTA_MAXIMUS_EXEMPLA).

### Q: Why is the help text in English if everything else is in Latin?

The error messages are in Latin. The help format ("USUS:", "OPTIONES:") can be whatever you want - it's just labels. The actual descriptions come from what you pass in.

### Q: Can I customize the help format?

No. It's fixed. If you need custom help, don't use `imprimere_auxilium()`. Generate your own.

### Q: Is this thread-safe?

No. Parse arguments once at startup from the main thread.

### Q: What if I want subcommands (like `git commit`, `git push`)?

This library doesn't support subcommands. You'd have to implement that yourself using positionals.

---

That's argument parsing. Define your arguments, parse them, use them. If you can't figure this out, I can't help you.
