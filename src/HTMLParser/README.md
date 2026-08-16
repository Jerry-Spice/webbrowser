# HTML Parser

Since HTML documents are the only kind of thing I want to support for this that means I need some kind of HTML parser in order to grab the relevant text to render. 

I am aware that there are tools that exist to do this however I want to personally take a crack at it.

---

## Structure

Since the HTML DOM is a tree that means I'm going to implement a Tree data structure that has some number of children. 

My vision is that this system should be object oriented and that every element inside the tree is inherited from some `HTMLElement` class.

## Notes

I found some information about parsing html and it involves much more than I actually thought in order to properly structure an HTML document into a DOM.

- [Syntax](https://www.w3.org/TR/2011/WD-html5-20110113/syntax.html)
- [Parsing](https://www.w3.org/TR/2011/WD-html5-20110113/parsing.html)
- [Tokenizing](https://www.w3.org/TR/2011/WD-html5-20110113/tokenization.html)

I want to implement a subset of this functionality because it seems like this definition also inherently supports Javascript. I don't want to support Javascript right now.

