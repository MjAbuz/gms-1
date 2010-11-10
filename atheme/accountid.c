#include "atheme.h"

DECLARE_MODULE_V1
(
        "groupserv/accountid", false, _modinit, _moddeinit,
        "$Revision$",
        "Stephen Bennett <stephen -at- freenode.net>"
);

static void gs_cmd_accountid(sourceinfo_t *si, int parc, char *parv[]);

command_t gs_accountid = { "ACCOUNTID", N_("Retrieves or modifies the GMS account ID"), "special:GMS", 2, gs_cmd_accountid, {} };

void _modinit(module_t *m)
{
    service_named_bind_command("GroupServ", &gs_accountid);
}

void _moddeinit()
{
    service_named_unbind_command("GroupServ", &gs_accountid);
}

void gs_cmd_accountid(sourceinfo_t *si, int parc, char *parv[])
{
    myuser_t *mu = myuser_find(parv[0]);
    if (!mu)
    {
        command_fail(si, fault_badparams, "No such account %s", parv[0]);
        return;
    }

    metadata_t *md;

    if (parc > 1)
    {
        // Setting...
        logcommand(si, CMDLOG_LOGIN, "set accountid for %s to %s", parv[0], parv[1]);
        md = metadata_add(mu, "private:gms:accountid", parv[1]);
    }
    else
    {
        logcommand(si, CMDLOG_LOGIN, "get accountid for %s", parv[0]);
        md = metadata_find(mu, "private:gms:accountid");
    }

    if (!md)
    {
        command_fail(si, fault_nosuch_key, "%s has no account ID.", parv[0]);
        return;
    }

    command_success_string(si, md->value, "Account ID for %s is %s", parv[0], md->value);
}
