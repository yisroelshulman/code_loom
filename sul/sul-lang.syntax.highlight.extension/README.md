# sul-lang README

The sul-lang extension is a syntax highlighter for the sul language to make it easier to read the files and see where things begin and end.


## Features

The syntax highlighter uses the following breakdown to differentiate the highlight coloring:
* `comment`
* `check`
* `input`, `output`
* `num`, `str`, `bool`
* `list`
* `true`, `false`
* `string`
* `number`


## What's in the sul-lang.syntax.highlighter.extension folder

* This folder contains all of the files necessary for the extension.
* `package.json` - this is the manifest file in which we declare the language support and define the location of the grammar file that has been copied into your extension.
* `syntaxes/sul.tmLanguage.json` - this is the Text mate grammar file that is used for tokenization.
* `language-configuration.json` - this is the language configuration, defining the tokens that are used for comments and brackets.

## Installing the extension

* To start using your extension with Visual Studio Code copy it into the `<user home>/.vscode/extensions` folder and restart Code.


`Note:` This syntax highlighter extension was created by the `yo generate-code` command, and modified later to fit the specifications for the sul language.