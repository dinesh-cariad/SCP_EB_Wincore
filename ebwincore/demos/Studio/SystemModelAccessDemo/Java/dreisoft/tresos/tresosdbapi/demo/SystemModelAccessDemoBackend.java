/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */
package dreisoft.tresos.tresosdbapi.demo;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.context.ECUConfigContext;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;
import dreisoft.tresos.tresosdb.api.model.DataModelAccess;
import dreisoft.tresos.tresosdb.api.model.IEntity;
import dreisoft.tresos.tresosdb.api.model.ProjectAccess;
import dreisoft.tresos.tresosdb.api.model.v2.autosartoplevelstructure.IAUTOSAR;
import dreisoft.tresos.tresosdb.api.model.v2.factory.TresosDBFactory;
import dreisoft.tresos.tresosdb.api.model.v2.genericstructure.generaltemplateclasses.arpackage.IARPackage;
import dreisoft.tresos.tresosdbapi.demo.generated.operationstatus.SystemModelAccessDemoOperationStatus;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * This class performs simple tasks which access the system model, also known as the tresosDB. It uses a Guided
 * Configuration API wizard. In order to perform those tasks it uses {@link TresosDBFactory}, {@link ProjectAccess} and
 * {@link DataModelAccess}, which are the main entry points of the tresosDB API.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 26.0
 */
public class SystemModelAccessDemoBackend extends AbstractBackend
{
    private static final String DEMOPACKAGE1_NAME = "demoPackage1";
    private static final String DEMOPACKAGE2_NAME = "demoPackage2";
    private static final String DEMOPACKAGE3_NAME = "demoPackage3";

    /**
     * Initialized within {@link #checkAddDemoPackages(APIOperationStatus)}.
     */
    private IARPackage demoPackage1;

    /**
     * Runs the selected demo parts.
     *
     * @return status messages including the query results
     */
    @Override
    public List<APIOperationStatus> doRun()
    {
        // prepare a list of result status messages
        ArrayList<APIOperationStatus> messages = new ArrayList<>();

        // prepare some demo data
        APIOperationStatus status = APIOperationStatus.getOkStatus();
        checkAddDemoPackages(status);
        if (!status.isOk())
        {
            messages.add(status);
            return messages;
        }

        boolean isGetChildEntitiesOfRootsSelected = getMemento().getBoolean(SystemModelAccessDemoPage.WidgetAndMementoIds.CHECKBOX_GET_CHILD_ENTITIES_OF_ROOTS.name());
        if (isGetChildEntitiesOfRootsSelected)
        {
            runGetChildEntitiesOfModelRootsDemo(messages);
        }

        boolean isGetShortNameChildEntitySelected = getMemento().getBoolean(SystemModelAccessDemoPage.WidgetAndMementoIds.CHECKBOX_GET_SHORTNAME_CHILD_ENTITY.name());
        if (isGetShortNameChildEntitySelected)
        {
            runGetShortNameChildEntityDemo(messages);
        }

        boolean isGetShortNamePathSelected = getMemento().getBoolean(SystemModelAccessDemoPage.WidgetAndMementoIds.CHECKBOX_GET_SHORTNAME_PATH.name());
        if (isGetShortNamePathSelected)
        {
            runGetShortNamePathDemo(messages);
        }
        return messages;
    }

    /**
     * Adds some demo packages unless already there.
     *
     * @see #getV2ModelRootFromCurrentProject(APIOperationStatus)
     * @see #checkAddToplevelPackage(IAUTOSAR, String)
     */
    private void checkAddDemoPackages(APIOperationStatus status)
    {
        IAUTOSAR root = getV2ModelRootFromCurrentProject(status);
        if (root == null)
        {
            return;
        }

        // create packages unless already existing
        demoPackage1 = checkAddToplevelPackage(root, DEMOPACKAGE1_NAME);
        checkAddToplevelPackage(root, DEMOPACKAGE2_NAME);
        checkAddToplevelPackage(root, DEMOPACKAGE3_NAME);
    }

    /**
     * Add a top-level package with the specified name, unless already existing.<br>
     * Demonstrates how access a top-level package from a given AUTOSAR root entity via it's short-name, and how to
     * create a new {@linkplain IARPackage} entity using the {@linkplain TresosDBFactory} and add it to the root entity.
     */
    private IARPackage checkAddToplevelPackage(IAUTOSAR root, String packageShortName)
    {
        Map<String, ? extends IARPackage> toplevelPackages = root.getArPackages();

        // key: short-name
        IARPackage arPackage = toplevelPackages.get(packageShortName);
        if (arPackage == null)
        {
            arPackage = TresosDBFactory.getInstance().createARPackage(packageShortName);
            root.addArPackage(arPackage);
        }
        return arPackage;
    }

    /**
     * Demonstrates how to fetch direct child entities from all system model roots of the current project via
     * {@linkplain DataModelAccess#getChildEntities(IEntity, boolean)}.
     *
     * @param messages to add some result status information to
     * @see #getRootEntitiesFromCurrentProject(APIOperationStatus)
     */
    private void runGetChildEntitiesOfModelRootsDemo(ArrayList<APIOperationStatus> messages)
    {
        // add top-level result status
        APIOperationStatus status = SystemModelAccessDemoOperationStatus.MESSAGE("Fetching child entities of system model roots.");
        messages.add(status);

        List<IEntity> rootEntities = getRootEntitiesFromCurrentProject(status);

        for (IEntity root : rootEntities)
        {
            APIOperationStatus rootEntityStatus = SystemModelAccessDemoOperationStatus.MESSAGE("Root entity: " + root.toString());
            status.addStatus(rootEntityStatus);

            boolean recurse = false;
            List<IEntity> childEntities = DataModelAccess.getChildEntities(root, recurse);

            // add the result to the status tree
            for (IEntity childEntity : childEntities)
            {
                rootEntityStatus.addStatus(SystemModelAccessDemoOperationStatus.MESSAGE("Child entity: " + childEntity.toString()));
            }
        }
    }

    /**
     * Demonstrates how to retrieve a short-name child entity of an entity via
     * {@linkplain DataModelAccess#getShortNameChildEntity(IEntity, String)}.
     *
     * @param messages to add some result status information to
     * @see #getRootEntitiesFromCurrentProject(APIOperationStatus)
     */
    private void runGetShortNameChildEntityDemo(ArrayList<APIOperationStatus> messages)
    {
        // add top-level result status
        APIOperationStatus status = SystemModelAccessDemoOperationStatus.MESSAGE("Fetching child entity with short-name '" + DEMOPACKAGE1_NAME + "' from the system model root for AUTOSAR 3.2 and above.");
        messages.add(status);

        List<IEntity> rootEntities = getRootEntitiesFromCurrentProject(status);

        for (IEntity root : rootEntities)
        {
            if (root instanceof IAUTOSAR)
            {
                // only interested in the V2 model root (for AUTOSAR 4 and above)
                SystemModelAccessDemoOperationStatus rootEntityStatus = SystemModelAccessDemoOperationStatus.MESSAGE("Found root entity: " + root.toString());
                status.addStatus(rootEntityStatus);
                IEntity childEntityNamedDemoPack1 = DataModelAccess.getShortNameChildEntity(root, DEMOPACKAGE1_NAME);

                if (childEntityNamedDemoPack1 != null)
                {
                    rootEntityStatus.addStatus(SystemModelAccessDemoOperationStatus.MESSAGE("Found child entity: " + childEntityNamedDemoPack1.toString()));
                }
                else
                {
                    rootEntityStatus.addStatus(SystemModelAccessDemoOperationStatus.MESSAGE("No child entity with short-name '" + DEMOPACKAGE1_NAME + "' found."));
                }
            }
            else
            {
                status.addStatus(SystemModelAccessDemoOperationStatus.MESSAGE("Ignoring model root: " + root.toString()));
            }
        }
    }

    /**
     * Demonstrates how to retrieve the short-name path of an entity via {@link DataModelAccess#getASPath(IEntity)}.
     *
     * @param messages to add some result status information to
     */
    private void runGetShortNamePathDemo(ArrayList<APIOperationStatus> messages)
    {
        String autosarShortNamePath = DataModelAccess.getASPath(demoPackage1);
        messages.add(SystemModelAccessDemoOperationStatus.MESSAGE("Short-name path of " + demoPackage1.toString() + " is: " + autosarShortNamePath));
    }

    /**
     * Demonstrates how to retrieve all root entities of the models via
     * {@link DataModelAccess#getRootsFromAutosarModel(DCtxt)} and the datamodel context of the current project.
     *
     * @return never null, maybe empty
     * @see #getDCtxtOfCurrentProject()
     */
    private List<IEntity> getRootEntitiesFromCurrentProject(APIOperationStatus status)
    {
        DCtxt dCtxtOfCurrentProject = getDCtxtOfCurrentProject();
        List<IEntity> rootsFromAutosarModel = DataModelAccess.getRootsFromAutosarModel(dCtxtOfCurrentProject);
        if (rootsFromAutosarModel.isEmpty())
        {
            status.addStatus(SystemModelAccessDemoOperationStatus.MESSAGE("No root entities found in current project!?"));
        }

        return rootsFromAutosarModel;
    }

    /**
     * Demonstrates how to retrieve the root entity of a class {@link IAUTOSAR} via
     * {@linkplain ProjectAccess#getRootEntity(String, Class, APIOperationStatus)} by the name of the current project.
     *
     * @return the root entity of the V2 (i.e. for AUTOSAR 4 and above) model of the currently selected project; null on
     *         error (check status)
     */
    private IAUTOSAR getV2ModelRootFromCurrentProject(APIOperationStatus status)
    {
        ECUConfigContext ecuConfigContext = (ECUConfigContext)getContexts().get(ECUConfigContext.ID);
        String projectName = ecuConfigContext.getAttribute(ECUConfigContext.ATTR_PROJECT_NAME);

        IAUTOSAR rootEntity = (IAUTOSAR)ProjectAccess.getRootEntity(projectName, IAUTOSAR.class, status);
        return rootEntity;
    }

    /**
     * Demonstrates how to retrieve the datamodel context of the current project.
     */
    private DCtxt getDCtxtOfCurrentProject()
    {
        ECUConfigContext ecuConfigContext = (ECUConfigContext)getContexts().get(ECUConfigContext.ID);
        return ecuConfigContext.getDCtxt();
    }

}
