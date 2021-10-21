/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */
package dreisoft.tresos.tresosdbapi.demo;

import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTGroup;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;
import dreisoft.tresos.tresosdbapi.demo.generated.nls.SystemModelAccessDemoNLS;

import org.eclipse.swt.widgets.Composite;

/**
 * Single page for the System Model Access Demo unattended wizard.
 *
 * <p>
 * The backend implementation of this wizard can be found in {@link SystemModelAccessDemoBackend}.
 * </p>
 *
 * <p>
 * This demo implementation shows checkboxes which control which parts of the demo code are to be executed.
 * </p>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 26.0
 */
public class SystemModelAccessDemoPage extends AbstractPage
{
    static final String PAGE_ID = "SystemModelAccessDemoPage";

    enum WidgetAndMementoIds
    {
        BROWSER,
        GROUP,
        LABEL_GET_CHILD_ENTITIES_OF_ROOTS,
        CHECKBOX_GET_CHILD_ENTITIES_OF_ROOTS,
        LABEL_GET_SHORTNAME_CHILD_ENTITY,
        CHECKBOX_GET_SHORTNAME_CHILD_ENTITY,
        LABEL_GET_SHORTNAME_PATH,
        CHECKBOX_GET_SHORTNAME_PATH,
    }

    @Override
    public String doGetId()
    {
        return PAGE_ID;
    }

    @Override
    public String doGetTitle()
    {
        return SystemModelAccessDemoNLS.PAGE_TITLE();
    }

    @Override
    public String doGetDescription()
    {
        return SystemModelAccessDemoNLS.PAGE_DESCRIPTION();
    }

    /**
     * Render the GUI.
     *
     * <p>
     * This demo implementation shows a group with a title, a browser and three checkboxes which control which tasks
     * should be performed.
     * </p>
     *
     * @param parent The composite to place the GUI in
     */
    @Override
    protected void doCreateControls(Composite parent)
    {
        WidgetFactory factory = getWidgetFactory();

        SWTGroup group = factory.createGroup(parent,
                                             WidgetAndMementoIds.GROUP.name(),
                                             WidgetFactory.GroupDecoration.LINE,
                                             WidgetFactory.GroupLayout.VERTICAL,
                                             null);

        String browserText = "<body style='background-color:#CCCCCC'>" +
            "The following parts of the demo code can be executed:<br/>" +
            "<ul>" +
            "<li>" + SystemModelAccessDemoNLS.LABEL_CHECKBOX_GET_CHILD_ENTITIES_OF_ROOTS() + "</li>" +
            "<li>" + SystemModelAccessDemoNLS.LABEL_CHECKBOX_GET_SHORTNAME_CHILD_ENTITY() + "</li>" +
            "<li>" + SystemModelAccessDemoNLS.LABEL_CHECKBOX_GET_SHORTNAME_PATH() + "</li>" +
            "</ul>" +
            "</body>";

        factory.createBrowser(group,
                              WidgetAndMementoIds.BROWSER.name(),
                              WidgetAndMementoIds.BROWSER.name(),
                              browserText,
                              4,
                              -1,
                              -1,
                              75,
                              true);

        factory.createLabel(group, WidgetAndMementoIds.LABEL_GET_CHILD_ENTITIES_OF_ROOTS.name(), WidgetAndMementoIds.LABEL_GET_CHILD_ENTITIES_OF_ROOTS.name(), SystemModelAccessDemoNLS.LABEL_CHECKBOX_GET_CHILD_ENTITIES_OF_ROOTS());
        factory.createCheckBox(group, WidgetAndMementoIds.CHECKBOX_GET_CHILD_ENTITIES_OF_ROOTS.name(), WidgetAndMementoIds.CHECKBOX_GET_CHILD_ENTITIES_OF_ROOTS.name());
        factory.createSpacer(group, 2, 1);

        factory.createLabel(group, WidgetAndMementoIds.LABEL_GET_SHORTNAME_CHILD_ENTITY.name(), WidgetAndMementoIds.LABEL_GET_SHORTNAME_CHILD_ENTITY.name(), SystemModelAccessDemoNLS.LABEL_CHECKBOX_GET_SHORTNAME_CHILD_ENTITY());
        factory.createCheckBox(group, WidgetAndMementoIds.CHECKBOX_GET_SHORTNAME_CHILD_ENTITY.name(), WidgetAndMementoIds.CHECKBOX_GET_SHORTNAME_CHILD_ENTITY.name());
        factory.createSpacer(group, 2, 1);

        factory.createLabel(group, WidgetAndMementoIds.LABEL_GET_SHORTNAME_PATH.name(), WidgetAndMementoIds.LABEL_GET_SHORTNAME_PATH.name(), SystemModelAccessDemoNLS.LABEL_CHECKBOX_GET_SHORTNAME_PATH());
        factory.createCheckBox(group, WidgetAndMementoIds.CHECKBOX_GET_SHORTNAME_PATH.name(), WidgetAndMementoIds.CHECKBOX_GET_SHORTNAME_PATH.name());

        factory.layout(parent);
    }

}
