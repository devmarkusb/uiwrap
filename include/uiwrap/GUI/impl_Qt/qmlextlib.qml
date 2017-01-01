// Markus Borris, 2016
// This file is part of my uiwrap library.

//!
/**
    This file is the dummydata-equivalent of a model that hosts a whole library
    of qml extension functions.
    If a function is not safely callable in GUI only mode, it has a leading bool
    parameter isGUIonlyMode needed to be passed in.
    Actually it goes even further: if a function has no isGUIonlyMode you can
    expect it to be a no-op in GUI mode.
    Simple reason: in GUI-only mode you don't have C++, so the implementations
    necessarily have to differ and be chosen by the bool input.
    The other way round: if you absolutely need a certain functionality also in
    GUI mode, you need to watch out for a pendant function *with* bool parameter.

    *Important* implementation note.
        This file needs to always be kept in sync with its pendant qmlextlib.h
        file. Don't change it without checking it back to its source, which is
        <sdks>\uiwrap\include\uiwrap\GUI\impl_Qt\
        (where qmlextlib.h is located as well)*/
//! \file

import QtQuick 2.7


QtObject {
    //####################################################################################################################
    // string handling for files, urls

    //! Expects string, \returns string.
    function prependFile_urlSchemePrefix(file_without_prefix) {
        return file_without_prefix;
    }

    //! Expects string, \returns string.
    function removeFile_urlSchemePrefix(file_with_prefix) {
        return file_with_prefix;
    }

    //! Expects string, \returns string.
    function toNativeSeparators(file_with_mixed_separators) {
        return file_with_mixed_separators;
    }

    //! Expects string, \returns string.
    function fromNativeSeparators(file_with_mixed_separators) {
        return file_with_mixed_separators;
    }


    //####################################################################################################################
    // file system

    //! Expects full path as string with slashes only, \returns bool.
    function fileExists(fullpath) {
        return true;
    }

    //! Expects full path as string with slashes only, \returns bool.
    function isFile(fullpath) {
        return true;
    }


    //####################################################################################################################
}
