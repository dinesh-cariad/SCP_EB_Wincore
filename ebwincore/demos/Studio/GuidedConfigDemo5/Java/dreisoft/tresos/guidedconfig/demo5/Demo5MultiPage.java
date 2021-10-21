/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo5;

import dreisoft.tresos.guidedconfig.api.gui.page.AbstractMultiPage;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;

import java.util.List;
import java.util.Vector;

/**
 * This MultiPage manages 3 single pages.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo5MultiPage extends AbstractMultiPage
{

    /**
     * The list of pages that forms the GUI
     */
    private List<AbstractPage> m_pages = null;

    @Override
    public String doGetDescription()
    {
        return "multi page description";
    }

    @Override
    public String doGetId()
    {
        return IDemo5Constants.PAGE_ID_MULTI;
    }

    @Override
    public String doGetTitle()
    {
        return "Wizard with multiple pages";
    }

    /**
     * This method is called to query the first page of the wizard.
     *
     * <p>
     * This implementation instantiates all sub-pages as the list of pages is statically defined in the XML file.
     * </p>
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.page.AbstractMultiPage#doGetFirstPage()
     * @since 2009.a
     */
    @Override
    public AbstractPage doGetFirstPage()
    {
        if (m_pages != null)
        {
            return m_pages.size() > 0 ? m_pages.get(0) : null;
        }
        return getNextPage();
    }

    /**
     * This method returns the next page in a multi-page GUI.
     *
     * <p>
     * This method is called by the wizard when the next button is pressed or to query the list of tabs in an editor.
     * </p>
     *
     * <p>
     * The pages are defined statically in an XML file
     * </p>
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.page.AbstractMultiPage#doGetNextPage()
     * @since 2009.a
     */
    @Override
    public AbstractPage doGetNextPage()
    {
        return getNextPage();
    }

    /**
     * Query to find out if the wizard has additional pages after the currently shown.
     *
     * @see dreisoft.tresos.guidedconfig.api.gui.page.AbstractMultiPage#doHasNextPage()
     * @since 2009.a
     */
    @Override
    public boolean doHasNextPage()
    {
        return getNextPage() != null;
    }

    /**
     * Determine the next page after the current page.
     *
     * <p>
     * If no page is the current page the first one is returned
     * </p>
     *
     * @return the next page or null if there are no more pages
     * @since 2009.a
     */
    private AbstractPage getNextPage()
    {
        createPages();
        AbstractPage page = getCurrentPage();
        if (page == null)
        {
            return m_pages.size() > 0 ? m_pages.get(0) : null;
        }
        int idx = m_pages.indexOf(page);
        if (idx == -1)
        {
            return null;
        }
        try
        {
            return m_pages.get(idx + 1);
        }
        catch (IndexOutOfBoundsException e)
        {
            return null;
        }
    }

    /**
     * Called by {@link #getNextPage()} to create all sub-pages by scanning the parsed model.
     *
     * @since 2009.a
     */
    private void createPages()
    {
        if (m_pages != null)
        {
            return;
        }
        m_pages = new Vector<AbstractPage>();
        m_pages.add(new Demo5SinglePage1());
        m_pages.add(new Demo5SinglePage2());
        m_pages.add(new Demo5SinglePage3());
    }

}
