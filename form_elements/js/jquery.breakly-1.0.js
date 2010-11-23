/**
 * jQuery Breakly plugin - Breaks your texts. Gently.
 * This plugin can be used to give the browser an "hint" on when (and eventually how) break
 * some long texts that are wrapped in a container with an explicitely defined width.
 * It works adding a "special" unicode character after the given number of characters.
 * By default the plugin inserts U+200B (the zero width space), but you can specify any
 * other character as the second parameter
 *
 * @name jquery-breakly-1.0.js
 * @author Claudio Cicali - http://claudio.cicali.name
 * @version 1.0
 * @date December 22, 2009
 * @category jQuery plugin
 * @copyright (c) 2009 Claudio Cicali ( http://claudio.cicali.name )
 * @license CC Attribution-No Derivative Works 2.5 Brazil - http://creativecommons.org/licenses/by-nd/2.5/br/deed.en_US
 * @examples
 *   $('h3').breakly(3); // "breaks" any h3 text (and any h3's children text too) inserting a \U+200B after every 3 characters
 *   $('h3').breakly(3, 0x202f); // Same as above, but inserts a "NARROW NO-BREAK SPACE" (just for the fun of it)
 
 * Visit http://lab.web20.it/breakly/example.html
 * List of Unicode spaces: http://www.cs.tut.fi/~jkorpela/chars/spaces.html
 */
$.fn.breakly = function(chopAt, spaceCode) {
  spaceCode |= 8203;  // U+200B ZERO WIDTH SPACE
  var zw = String.fromCharCode(spaceCode), re = new RegExp(/\B/), orig, idx, chopped, ch;
  function breakly(node) {
    if (3 == node.nodeType && (orig = node.nodeValue).length > chopAt) {
      idx = 0;
      chopped=[];
      for (var i=0; i < orig.length; i++) {
        ch = orig.substr(i,1);
        chopped.push(ch);
        if (null != ch.match(re)) {
          idx=0;
          continue;
        }
        if (++idx == chopAt) {
          ch = orig.substr(i+1,1); // look ahead
          if (ch && null == ch.match(re)) {
            chopped.push(zw);
            idx=0;
          }
        }
      }
      node.nodeValue = chopped.join('');
    } else {
      for (var i=0; i < node.childNodes.length; i++) {
        breakly(node.childNodes[i]);
      }
    }
  }

  return this.each(function() {
    breakly(this);
  })
}


