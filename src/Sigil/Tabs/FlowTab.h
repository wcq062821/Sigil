/************************************************************************
**
**  Copyright (C) 2009, 2010, 2011  Strahinja Markovic  <strahinja.markovic@gmail.com>
**  Copyright (C) 2012 John Schember <john@nachtimwald.com>
**
**  This file is part of Sigil.
**
**  Sigil is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Sigil is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Sigil.  If not, see <http://www.gnu.org/licenses/>.
**
*************************************************************************/

#pragma once
#ifndef FLOWTAB_H
#define FLOWTAB_H

#include <QtCore/QUrl>

#include "MainUI/MainWindow.h"
#include "Tabs/ContentTab.h"
#include "Tabs/WellFormedContent.h"

class QSplitter;
class BookViewEditor;
class BookViewPreview;
class CodeViewEditor;
class ViewEditor;
class Resource;
class HTMLResource;
class QUrl;
class WellFormedCheckComponent;

/**
 * A tab widget used for displaying XHTML chapters.
 * It can display the chapter in both rendered view (Book View)
 * and raw code view (Code View).
 */
class FlowTab : public ContentTab, public WellFormedContent
{
    Q_OBJECT

public:

    /**
     * Constructor.
     * 
     * @param resource The resource this tab will be displaying.
     * @param fragment The URL fragment ID to which the tab should scroll.
     * @param view_state In which View should the resource open or switch to.
     * @param line_to_scroll_to To which line should the resource scroll.
     * @param parent The parent of this QObject.
     */
    FlowTab( HTMLResource& resource, 
             const QUrl &fragment, 
             MainWindow::ViewState view_state,
             int line_to_scroll_to = -1,
             QWidget *parent = 0 );

    ~FlowTab();

    void RestoreCaret();

    // Overrides inherited from ContentTabs

    bool IsModified();

    bool PrintEnabled();

    bool CutEnabled();

    bool CopyEnabled();

    bool PasteEnabled();

    int GetCursorLine() const;
    int GetCursorColumn() const;

    float GetZoomFactor() const;

    void SetZoomFactor( float new_zoom_factor );

    void UpdateDisplay();

    Searchable* GetSearchableContent();

    void SetViewState( MainWindow::ViewState new_view_state );

    bool IsLoadingFinished();

    /**
     * Scrolls the tab to the specified fragment (if in Book View).
     *
     * @param fragment The URL fragment ID to which the tab should scroll.
     */
    void ScrollToFragment( const QString &fragment );

    /**
     * Scrolls the tab to the specified line (if in Code View).
     *
     * @param line The line to scroll to.
     */
    void ScrollToLine( int line );

    /**
     * Scrolls the tab to the top.
     */
    void ScrollToTop(); 

    // Overrides inherited from WellFormedContent

    void AutoFixWellFormedErrors();

    bool GetCheckWellFormedErrors();

    void SetWellFormedDialogsEnabledState( bool enabled );

    void SetCheckWellFormedErrorsState( bool enabled );

    void TakeControlOfUI();

    QString GetFilename();

public slots:

    bool IsDataWellFormed();

    /**
     * Implements Undo action functionality.
     */
    void Undo();

    /**
     * Implements Redo action functionality.
     */ 
    void Redo();

    /**
     * Implements Cut action functionality.
     */
    void Cut();

    /**
     * Implements Copy action functionality.
     */
    void Copy();

    /**
     * Implements Paste action functionality.
     */
    void Paste();

    /**
     * Implements Split chapter action functionality.
     */
    void SplitChapter();

    /**
     * Implements Insert SGF chapter marker action functionality.
     */
    void InsertSGFChapterMarker();

    /**
     * Implements Split on SGF chapter markers action functionality.
     */
    void SplitOnSGFChapterMarkers();

    /**
     * Implements \em a \em part of Insert image action functionality. 
     * The rest is in MainWindow. It has to be, FlowTabs don't
     * have a reference to the Book object.
     *
     * @param image_path The full path to the image that should be inserted.
     */
    void InsertImage( const QString &image_path );

    /**
     * Implements Print Preview action functionality.
     */
    void PrintPreview();

    /**
     * Implements Print action functionality.
     */
    void Print();

    /**
     * Implements Book View action functionality.
     */
    void BookView();

    /**
     * Implements Split View action functionality.
     */
    void SplitView();

    /**
     *  Implements Code View action functionality.
     */
    void CodeView();
    
    // inherited 

    void SaveTabContent();

    void LoadTabContent();

signals:

    /**
     * Emitted when the tab enters the Book View.
     */
    void EnteringBookView();

    void EnteringBookPreview();

    /**
     * Emitted when the tab enters the Code View.
     */
    void EnteringCodeView();

    /**
     * Emitted when the View changes.
     */
    void ViewChanged();    

    /**
     * Emitted when the selection in the view has changed.
     */
    void SelectionChanged();

    /**
     * Emitted when a linked is clicked in the Book View.
     *
     * @param url The URL of the clicked link.
     */
    void LinkClicked( const QUrl &url );

    /**
     * Emitted when an "old" tab should be created.
     * Emitted as part of the chapter break operation.
     *
     * @param content The content of the "old" tab/resource.
     * @param originating_resource  The original resource from which the content
     *                              was extracted to create the "old" tab/resource.
     */
    void OldTabRequest( QString content, HTMLResource& originating_resource );

    /**
     * Emitted when the user wants to create several new chapters.
     *
     * @param chapters The content of the new chapters to be created.
     * @param originating_resource The original HTML chapter that chapters
     * will be created after.
     */
    void NewChaptersRequest( QStringList chapters, HTMLResource &originating_resource );

    /**
     * Emitted when the state of the Book/Code/Split View buttons has changed.
     */
    void ViewButtonsStateChanged();

private slots:

    /**
     * Performs the delayed initialization of the tab.
     * We perform delayed initialization after the widget is on
     * the screen. This way, the user perceives less load time.
     */
    void DelayedInitialization();

    /**
     * Emits the ContentChanged signal.
     */
    void EmitContentChanged();  

    void EmitUpdateCursorPosition();

    /**
     * Receives the signal emitted when an editor loses focus. Ensures that
     * the editor's content is well-formed and then saves it.
     *
     * @param A pointer to the editor.
     */
    void LeaveEditor( QWidget *editor );

    /** 
     * Receives the signal emitted when an editor gains focus. Ensures that
     * the editor is displaying the correct content.
     *
     * @param A pointer to the editor.
     */
    void EnterEditor( QWidget *editor );

    /**
     * Receives the signal emitted when user settings have changed.
     */
    void LoadSettings();

    // Called when the underlying resource is modified. It is only connected
    // when the view state is BV and used to know if BV should be reloaded
    // when the user enters the view. CV is linked to the resource in such a
    // way that this is unnecessary. The CV linking is not possible in BV.
    void ResourceModified();

private:

    /**
     * Makes the Book View the current View.
     */
    void EnterBookView();

    void EnterBookPreview();

    /**
     * Makes the Code View the current View.
     */
    void EnterCodeView();

    /**
     * Reads all the stored application settings like
     * window position, geometry etc.
     */
    void ReadSettings();
    
    /**
     * Writes all the stored application settings like
     * window position, geometry etc.
     */
    void WriteSettings();

    /**
     * Returns the active View Editor.
     *
     * @return The active View Editor.
     */
    ViewEditor& GetActiveViewEditor() const;

    /**
     * Connects all the required signals to their respective slots.
     */
    void ConnectSignalsToSlots();


    ///////////////////////////////
    // PRIVATE MEMBER VARIABLES
    ///////////////////////////////

    /**
     * The fragment to scroll to after the tab is initialized.
     */
    const QUrl m_FragmentToScroll;

    /**
     * The line to scroll to after the tab is initialized.
     */
    int m_LineToScrollTo;

    /**
     * The HTML resource the tab is currently displaying.
     */
    HTMLResource &m_HTMLResource;

    /**
     * The splitter widget that separates the two Views.
     */
    QSplitter &m_Splitter;

    /**
     * The Book View Editor.
     * Displays and edits the rendered state of the HTML.
     */
    BookViewEditor &m_wBookView;

    BookViewPreview &m_wBookPreview;

    /**
     * The Code View Editor.
     * Displays and edits the raw code.
     */ 
    CodeViewEditor &m_wCodeView;

    /**
     * This is used in a few different ways.
     *
     * 1) We store the requested view state for loading the document.
     * 2) We store the current view state.
     * 3) We compare the state of the view when entering to this in order
     *    to determine if we have changed the view (BV, CV) in order to
     *    load the latest content into the view.
     */
    MainWindow::ViewState m_ViewState;

    /**
     * The component used to display a dialog about 
     * well-formedness errors.
     */
    WellFormedCheckComponent& m_WellFormedCheckComponent;

    /**
     * A flag to be used in conjunction with the check for well-formedness which
     * indicates whether it's safe to reload the tab content.
     */
    bool m_safeToLoad;

    bool m_initialLoad;

    bool m_BookViewNeedReload;
};

#endif // FLOWTAB_H


