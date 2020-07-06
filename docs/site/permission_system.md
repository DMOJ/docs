# Permission System

The DMOJ's permission system is very extensive and allows fine-tuning a user's permissions. Here, we will document which permissions are required to perform certain tasks on the site. Any undocumented models means that they follow Django's default permission system of (`can_add_<model>`, `can_change_<model>`, `can_delete_<model>`, and `can_view_<model>`).

## Blog Posts
Blog posts follow Django's default permission system.

#### `edit_all_post` (Edit all posts)
**Prerequisite permissions**: `change_blogpost`

The user can edit all blog posts.

## Comments
Comments and comment locks follow Django's default permission system.

#### `override_comment_lock` (Override comment lock)
The user can post comments on pages which are comment locked.

## Contests
Contest participations, contest problems, contest submissions, and contest tags follow Django's default permission system.

#### `see_private_contest` (See private contests)
The user can see all contests without explicitly set as organizer. The user will also be able to see hidden scoreboards. However, they will *not* be able to edit contests on the admin site.

#### `edit_own_contest` (Edit own contests)
The user can edit contests on the admin site, but only if they are explicitly set as organizer.

#### `edit_all_contest` (Edit all contests)
**Prerequisite perrmisions: `edit_own_contest`**

**Superceded permissions: `see_private_contest`**

The user can see and edit all contests on the admin site without explicitly set as organizer.

#### `clone_contest` (Clone contest)
The user can clone contests that they can edit.

#### `moss_contest` (MOSS contest)
The user can MOSS contests that they can edit.

#### `contest_rating` (Rate contests)
The user can edit rating-related fields on the contest admin page, and rate the contest.

#### `contest_access_code` (Contest access codes)
The user can edit the `access_code` field on the contest admin page.

#### `create_private_contest` (Create private contests)
The user can create organization-private and user-private contests. The user can also edit the `is_visible` field 
on the contest admin page as long as the contest is organization-private or user-private.

#### `change_contest_visibility` (Change contest visibility)
The user can edit the `is_visible` field on the contest admin page.

#### `contest_problem_label` (Edit contest problem label script)
The user can edit the contest problem label script on the contest admin page.

## Organizations
Organizations follow Django's default permission system.

#### `organization_admin` (Administer organizations)
The user can edit `registrant`, `admins`, `is_open`, `slots` fields on the admin page.

#### `edit_all_organization` (Edit all organizations)
**Prerequisite permissions: `change_organization`**

The user can edit all organizations.

## Problems

#### `see_organization_problem` (See organization-private problems)
The user can see organization-private (but public) problems.

#### `see_private_problem` (See hidden problems)
**Superceded permissions: `see_organization_problem`**

The user can see all problems. However, they will *not* be able to edit problems on the admin site or view submission source code.

#### `edit_own_problem` (Edit own problems)
The user can edit problems on the admin site, but only if they are explicitly set as author or curator. They will also be able to view submission source code for these problems.

#### `edit_public_problem` (Edit public problems)
**Prerequisite perrmisions: `edit_own_problem`**

The user can edit problems on the admin site, but only if the problem is public. They will also be able to view submission source code for these problems.

#### `edit_all_problem` (Edit all problems)
**Prerequisite perrmisions: `edit_own_problem`**

**Superceded permissions: `see_private_problem`, `edit_public_problem`, `view_all_submission`**

The user can see and edit all problems on the admin site.

#### `clone_problem` (Clone problem)
The user can clone problems that they can edit.

#### `change_public_visibility` (Change is_public field)
The user can change the `is_public` field.

#### `change_manually_managed` (Change is_manually_managed field)
The user can change the `is_manually_managed` field.

### Problem Solutions
Problem solutions follow Django's default permission system.

#### `see_private_solution` (See hidden solutions)
The user can see all problem solutions for problems they can access, regardless of if the solution is public or not.

## Profile
Profiles follow Django's default permission system.    

#### `totp` (Edit TOTP settings)
The user can edit TOTP-related fields.

## Submissions

Submission visibility and editability are determined by problem permissions. 
If the user can edit the problem, they can also edit related submissions.
No user can add submissions, and deleting permissions follow Django's default permission system.


#### `abort_any_submission` (Abort any submission)
The user can abort any submission. Without this permission, the user can only abort 
submissions that they have submitted and which have not been rejudged.

#### `rejudge_submission` (Rejudge the submission)

**Prerequisite permissions: `edit_own_problem`**

The user can rejudge submissions for problems they can edit.

#### `rejudge_submission_lot` (Rejudge a lot of submissions)

**Prerequisite permissions: `rejudge_submission`**

The user can batch-rejudge submissions, and bypass the `DMOJ_SUBMISSIONS_REJUDGE_LIMIT` setting.

#### `spam_submission` (Submit without limit)

The user can bypass the `DMOJ_SUBMISSION_LIMIT` setting, meaning they can have an infinite number of non-rejudged submissions queued.

#### `view_all_submission` (View all submission)

The user can view submissions for all problems, but *cannot* edit them on the admin site.

#### `resubmit_other` (Resubmit others' submission)

The user can resubmit submissions by other users.
