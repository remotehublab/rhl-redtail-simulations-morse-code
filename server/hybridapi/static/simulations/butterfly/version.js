/*
 * This file only contains a global window.cacheBuster variable.
 * The idea is that whenever we change this variable name, automatically
 * the web browser and the server will not use a cache and will retrieve the
 * latest version. The ideal situation is to use a hash of the current content
 * or the git version or something like that. The simplest version is just
 * using the date, and incrementing with every version. But the format does 
 * not matter as long as it works in "?cacheBuster=" + window.cacheBuster
 * (so do not use characters like '?' or '=', but if you are deploying 20
 * things in one day, feel free to add also time, or v1, v2 or whatever.
 */
window.cacheBuster = "2023-03-20c";
